#include "pch.h"
#include "shader_util.h"

namespace nshaders
{
	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// shader Program
		mProgramId = glCreateProgram();
		glAttachShader(mProgramId, vertex);
		glAttachShader(mProgramId, fragment);
		if (geometryPath != nullptr)
			glAttachShader(mProgramId, geometry);
		glLinkProgram(mProgramId);
		checkCompileErrors(mProgramId, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);

	}

	void Shader::checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}


	void Shader::use()
	{
		glUseProgram(mProgramId);
	}

	void Shader::unload()
	{
		glDeleteProgram(mProgramId);
	}

	void Shader::set_mat4(const glm::mat4& mat4, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::set_i1(int v, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glUniform1i(myLoc, v);
	}

	void Shader::set_f1(float v, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glUniform1f(myLoc, v);
	}

	void Shader::set_f3(float a, float b, float c, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glUniform3f(myLoc, a, b, c);
	}

	void Shader::set_vec3(const glm::vec3& vec3, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glProgramUniform3fv(get_program_id(), myLoc, 1, glm::value_ptr(vec3));
	}

	void Shader::set_vec4(const glm::vec4& vec4, const std::string& name)
	{
		GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
		glProgramUniform4fv(get_program_id(), myLoc, 1, glm::value_ptr(vec4));
	}

	void Shader::set_texture(int shader_param_id,int tex_type,unsigned int tex_id)
	{
		glActiveTexture(shader_param_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}

	
}

