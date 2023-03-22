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
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	void Attach(const std::string& vs, const std::string& fs);

	void SetUniformValue4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformValueMat4f(const std::string& name, const glm::mat4& matrix);
	bool IsValid() const;
private:
	void LinkAndValidate();

	bool GetUniformLocation(const std::string& name, int& location);

    static std::string ParseShader(const std::string& file_path);
    static bool CompileShader(GLenum type, const std::string& source, unsigned int& id);
};

