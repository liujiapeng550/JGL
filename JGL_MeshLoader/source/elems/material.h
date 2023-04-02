
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "pch.h"
#include "tinyxml2.h"
#include "utils/texturessystem.h"
#include "shader/shader_util.h"

using namespace std;
using namespace tinyxml2;

class Attribute {
public:
	string name;
	string value;
};

class Param {
public:
	string name;
	string type;
	string defaultValue;
	bool multipass;
};
glm::vec3 StringtoFloat3(std::string str)
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

class Material {
public:
	Material(const char* xmlPath);
	void print() {
		cout << "Name: " << name << endl;
		for (const auto& param : params) {
			cout << "Param Name: " << param.name << endl;
			cout << "Param Type: " << param.type << endl;
			cout << "Param Default Value: " << param.defaultValue << endl;
		}
	}
	bool update_shader_params(nshaders::Shader* shader);
	bool set_param(string name,string trye,string value);

private:
	vector<Param> params;
	string name;
	vector<unsigned int> mTexture_id_vec;
	map<string, float> mFloat_map;
	map<string, glm::vec3> mFloat3_map;
	bool mMultipass=false;
};

vector<string> split(const string& str, char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string token;
	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

