#pragma
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

struct Param {
	string name;
	string type;
	string defaultValue;
	bool multipass;
};


class Material {
public:
	Material();
	void load(const char* xmlPath);
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
	glm::vec3 StringtoFloat3(std::string str);
	string getshaderPath() { return mshader_path; }
	map<string, float>& getFloatMap() { return mFloat_map; }
private:
	vector<Param> params;
	string name;
	vector<unsigned int> mTexture_id_vec;
	map<string, float> mFloat_map;
	map<string, glm::vec3> mFloat3_map;
	bool mMultipass=false;
	string mshader_path;
};

inline vector<string> split(const string& str, char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string token;
	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

