#include "material.h"
#include "iostream"
#include<fstream>
#include<stdio.h>
//#include<string>
#include "param.h"
//str�ĽṹΪ����float ���硰1.0,1.0,1.0��
bool Material::StringTovalue(glm::vec3& value,const char* str) {
    char tmp[64];
    glm::vec3 res;
    //ָ��ָ��","��λ��
    const char* p = strchr(str, ',');
    if (p == 0)
    {
        return;
    }
    //strָ����p-str ��λ�õ��ַ�����ȡ������tmp��
    strncpy(tmp, str, p - str);
    tmp[p - str] = 0;
    res.r = (float)atof(tmp);
    //strָ��ָ���һ����������һ�����ַ�
    str = p+ 1;
    p = strchr(str, ',');
    if (p == 0) {
        return;
    }
    strncpy(tmp, str, p - str);
    tmp[p - str] = 0;
    res.g = (float)atof(tmp);

    str = p + 1;
    p = strchr(str, ',');
    if (p == 0) {
        return;
    }
    strncpy(tmp, str, p - str);
    res.b = (float)atof(tmp);
    value = res;
}

bool Material::LoadFromFile(const std::string xmlPath)
{
    pugi::xml_parse_result result = m_xmldoc.load_file(xmlPath.c_str());
    pugi::xpath_node_set shaderSet = m_xmldoc.select_nodes("/Materia/Params/Param");
    for (auto nodeIt = shaderSet.begin(); nodeIt != shaderSet.end(); ++nodeIt)
    {
        auto xpathNode = *nodeIt;
        
        if (xpathNode)
        {
            // Load default user shaders.
            //if (const char* xpathValue = xpathNode.node().attribute("Name").value())
            //{
            //    _shaderName = std::string(xpathValue);
            //}
            
            auto name = xpathNode.node().attribute("UiLabel").value();
            auto type = xpathNode.node().attribute("Type").value();
            if (type == "float") {
                const char* strValue = xpathNode.node().attribute("Value").value();
                glm::vec1 value = (glm::vec1)atof(strValue);
                Param p(type, name);
            }
            else if(type == "float3")
            {
                const char* strValue = xpathNode.node().attribute("Value").value();
                glm::vec3 value;
                StringTovalue(value, strValue);
            }
        }
    }
    return true;
}



        // pugixmltest.cpp : Defines the entry point for the console application.  
//  

//���������·�������߾���·��  
bool LoadFromFile(const char* szXmlFileName)
    {
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_file(szXmlFileName);

        std::cout << "Load result: " << result.description() << std::endl
            << "name.attrib = " << doc.child("Name").attribute("age").value() << std::endl;

        return true;
    }

    //bool LoadFromBuffer()
    //{

    //    const char* szBuffer = "<?xml version=\"1.0\"?><Name age=\"10\">good</Name>";
    //    pugi::xml_document doc;
    //    if (!doc.load_buffer(szBuffer, strlen(szBuffer)))
    //        return false;

    //    std::cout << "name.attrib = " << doc.child("Name").attribute("age").value() << std::endl;
    //    return true;

    //}
    //bool LoadFromMem()
    //{
    //    const char* szBuffer = "<?xml version=\"1.0\"?><Name age=\"10\">good</Name>";
    //    pugi::xml_document doc;
    //    if (!doc.load(szBuffer))
    //        return false;

    //    std::cout << "name.attrib = " << doc.child("Name").attribute("age").value() << std::endl;
    //    return true;
    //}
    //bool LoadFromStream()
    //{
    //    std::ifstream stream("tree.xml");
    //    pugi::xml_document doc;
    //    if (!doc.load(stream))
    //    {
    //        return false;
    //    }
    //    std::cout << "name.attrib = " << doc.child("Name").attribute("age").value() << std::endl;
    //    return true;
    //}

    int main(int argc, char* argv[])
    {
        //LoadFromFile("tree.xml");  
        //LoadFromBuffer();  
        Material matIns;
        std::ifstream stream("material.xml");
 
        
        matIns.LoadFromFile("material.xml");
        return 0;
    }