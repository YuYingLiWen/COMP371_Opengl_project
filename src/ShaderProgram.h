#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <string>

#include "GLObject.h"
#include "IBinder.h"
#include "Utils.h"



class ShaderProgram : public IBinder, GLObject
{
private:
	int* vs_id = nullptr;
	int* fs_id = nullptr;
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind() const override;
	void Unbind() const override;

	void Attach(GLenum type, const std::string& file_path);
	void LinkAndValidate();

	void SetUniformValue4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformValueMat4f(const std::string& name, const glm::mat4& matrix);

private:
	bool GetUniformLocation(const std::string& name, int& location);

    static std::string ParseShader(const std::string& file_path);
    static bool CompileShader(GLenum type, const std::string& source, unsigned int& id);
};

