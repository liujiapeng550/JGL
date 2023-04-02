#include "pch.h"
#include "material.h"


Material::Material(const char* xmlPath)
{
	XMLDocument doc;
	doc.LoadFile(xmlPath);
	XMLElement* root = doc.FirstChildElement("Material");
	name = root->Attribute("Name");
	istringstream(root->Attribute("multipass"))>>boolalpha>>mMultipass;
	for (XMLElement* child = root->FirstChildElement("Param"); child != NULL; child = child->NextSiblingElement("Param")) {
		Param param;
		param.name = child->Attribute("Name");
		param.type = child->Attribute("Type");
		param.defaultValue = child->Attribute("Default");
		params.push_back(param);

		if (param.type == "Texture") {
			unsigned int tex_id = TextureSystem::getTextureId(param.defaultValue.c_str());
			mTexture_id_vec.push_back(tex_id);
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
	if (mMultipass) {
		for (auto it : mFloat_map) {
			shader->set_f1(it.second, it.first);
		}
		for (auto it : mFloat3_map) {
			shader->set_vec3(it.second, it.first);
		}
		for (int i = 0; i < mTexture_id_vec.size(); i++) {
			shader->set_texture(GL_TEXTURE0 + i, GL_TEXTURE_2D, mTexture_id_vec[i]);
		}
	}
	return true;
}

bool Material::set_param(string name, string type,string value)
{
	if (type == "float") {
		mFloat_map[name] = stof(value);
	}
	if (type == "float3") {
		mFloat3_map[name] = StringtoFloat3(value);
	}
	return true;
}
