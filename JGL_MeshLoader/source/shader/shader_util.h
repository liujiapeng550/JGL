#pragma once

namespace nshaders
{

	// Utitlity class to load, compile and attach a vertex- and fragment shader to a program
	class Shader
	{

	public:
		// Load a vertex and a fragment shader from file
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		// Use the program
		void use();

		// Delete the program
		void unload();

		unsigned int get_program_id() { return mProgramId; }

		void set_mat4(const glm::mat4& mat4, const std::string& name);

		void set_i1(int v, const std::string& name);
		void set_f1(float v, const std::string& name);
		void set_f3(float a, float b, float c, const std::string& name);

		void set_vec3(const glm::vec3& vec3, const std::string& name);
		void set_vec4(const glm::vec4& vec4, const std::string& name);
		void set_texture(int shader_param_id, int tex_type, unsigned int tex_id);
	private:
		unsigned int mProgramId;
		void checkCompileErrors(GLuint shader, std::string type);
	};
}

