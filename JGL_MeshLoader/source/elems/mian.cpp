#include "tinyxml2.h"
#include "material.h"
int main() {
	XMLDocument doc;
	doc.LoadFile("example.xml");
	XMLElement* root = doc.FirstChildElement("Material");
	Material material;
	material.name = root->Attribute("Name");

	for (XMLElement* child = root->FirstChildElement("Param"); child != NULL; child = child->NextSiblingElement("Param")) {
		Param param;
		param.name = child->Attribute("Name");
		param.type = child->Attribute("Type");
		param.defaultValue = child->Attribute("Default");
		material.params.push_back(param);
	}

	material.print();

	return 0;
}