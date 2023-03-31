#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <string>

#include "Utils.h"



class ShaderProgram 
{
private:
	unsigned int program_id = -1;
	unsigned int vs_id = -1, fs_id = -1;
	bool valid = false;

	std::string name = "UNKOWN SHADER";
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	bool IsValid() const;
	void Attach(const std::string& vs, const std::string& fs);

	/// PVM
	void SetProjection(const glm::mat4& matrix);
	void SetView(const glm::mat4& matrix);
	void SetModel(const glm::mat4& matrix);
	void SetPVM(const glm::mat4& p, const glm::mat4& v, const glm::mat4& m);

	/// Set Uniforms
	void SetUniformInt(const std::string& name, int integer);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4 v4);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, glm::vec3 v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	void LinkAndValidate();


	bool GetUniformLocation(const std::string& name, int& location);

    static std::string ParseShader(const std::string& file_path);
    static bool CompileShader(GLenum type, const std::string& source, unsigned int& id);
};

