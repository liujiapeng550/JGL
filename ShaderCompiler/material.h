#pragma once
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "param.h"
class Material
{
public:
	bool StringTovalue(glm::vec3 &value,const char* str);
	bool LoadFromFile(static std::string filePath);
	bool Parse_xml();
private:
	pugi::xml_document m_xmldoc;
	std::vector<Param> m_params_vec;
};