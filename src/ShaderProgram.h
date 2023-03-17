#pragma once

#include "GLObject.h"
#include "IBinder.h"
#include "Renderer.h"

#include <string>
#include <fstream>
#include <sstream>
#include <memory>

class ShaderProgram : public IBinder, GLObject
{
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind() const override;
	void Unbind() const override;

	void Attach(GLenum type, const std::string& file_path);

	int GetUniformlocation(std::string name);

private:
    static std::string ParseShader(const std::string& file_path);
    static void CompileShader(GLenum type, const std::string& source, unsigned int& id, bool& validation);
};

