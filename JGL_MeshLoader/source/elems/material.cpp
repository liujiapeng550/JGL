#include "material.h"

Material::Material():name("default")
{
}



void Material::load(const char* xmlPath)
{
	XMLDocument doc;
	doc.LoadFile(xmlPath);
	XMLElement* root = doc.FirstChildElement("Material");
	mshader_path = FileSystem::getPath(root->Attribute("shader"));
	name = root->Attribute("Name");
	istringstream(root->Attribute("multipass")) >> boolalpha >> mMultipass;
	for (XMLElement* child = root->FirstChildElement("Param"); child != NULL; child = child->NextSiblingElement("Param")) {
		Param param;
		param.name = child->Attribute("Name");
		param.type = child->Attribute("Type");
		param.defaultValue = child->Attribute("Default");
		params.push_back(param);
		if (param.type == "Texture") {
			string path = FileSystem::getPath(param.defaultValue);
			unsigned int tex_id = TextureSystem::getTextureId(path.c_str());
			mTexture_map[param.name] = pair(tex_id,path);
		}
		if (param.type == "float") {
			mFloat_map.insert(pair<string, float>(param.name, stof(param.defaultValue)));
		}
		if (param.type == "float3")
		{
			vector<string> strlist;
			glm::vec3 f3 = glm::vec3(0.0);
			pair<string, glm::vec3> p(param.name, StringtoFloat3(param.defaultValue));
			mFloat3_map.insert(p);
		}
	}
}

bool Material::update_shader_params(nshaders::Shader* shader)
{
	for (auto it : mFloat_map) {
		shader->set_f1(it.second, it.first);
	}
	for (auto it : mFloat3_map) {
		shader->set_vec3(it.second, it.first);
	}
	int tex_index = 0;
	for (auto it : mTexture_map) {
		shader->set_i1(GL_TEXTURE0 + tex_index, it.first);
		shader->set_texture(GL_TEXTURE0 + tex_index, GL_TEXTURE_2D,it.second.first);
	}
	return true;
}

bool Material::set_param(string name, string type, string value)
{
	if (type == "float") {
		mFloat_map[name] = stof(value);
	}
	if (type == "float3") {
		mFloat3_map[name] = StringtoFloat3(value);
	}
	return true;
}

glm::vec3 Material::StringtoFloat3(std::string str)
{
	istringstream iss(str);
	string token;
	vector<string> tmp;
	while (getline(iss, token, ','))
	{
		tmp.push_back(token);
	}
	glm::vec3 res{ stof(tmp[0]),stof(tmp[1]),stof(tmp[2]) };
	return res;
}

Material::~Material() {
	// 清除参数列表
	params.clear();
	// 清除纹理映射
	mTexture_map.clear();
	// 清除浮点数映射
	mFloat_map.clear();
	// 清除浮点向量映射
	mFloat3_map.clear();
}

void Material::set_textures(map<string, pair<unsigned int, string>> textures)
{
	mTexture_map = textures;
}
