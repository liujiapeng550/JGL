#include <glm/glm.hpp>
#include <string>
union ParamValue
{
	std::string name;
	glm::vec1	Float;
	glm::vec3	Vector;
};
struct ShaderResource
{
	std::string Name;
	std::string Type;
	std::string Value;
};

class Param {
public:
	Param(const char* type, const char* name) : m_type(type), m_name(name) {}
	bool SetValue(ParamValue value) {
		switch(value.tok)
		if(value.typeid == )
		m_value = value;
	}
private:
	const char* m_type;
	const char* m_name;
	ParamValue m_value;
};