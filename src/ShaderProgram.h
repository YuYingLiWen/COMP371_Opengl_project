#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "GLObject.h"
#include "IBinder.h"
#include "Utils.h"



class ShaderProgram : public IBinder, GLObject
{
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind() const override;
	void Unbind() const override;

	void Attach(GLenum type, const std::string& file_path);

	void SetUniformValue4f(std::string name, float v0, float v1, float v2, float v3);
private:
    static std::string ParseShader(const std::string& file_path);
    static void CompileShader(GLenum type, const std::string& source, unsigned int& id, bool& validation);
};

