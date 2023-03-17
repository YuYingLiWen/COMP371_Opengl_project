#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <memory>

#define SHADER_TYPE(x) (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"


ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

void ShaderProgram::Bind() const
{
	glUseProgram(id);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

void ShaderProgram::Attach(GLenum type, const std::string& file_path)
{
    unsigned int shader_id;
    bool compilation_successful = false;

    std::string shader_source = ParseShader(file_path);

    if (type == GL_VERTEX_SHADER) CompileShader(GL_VERTEX_SHADER, shader_source, shader_id, compilation_successful);
    else if (type == GL_FRAGMENT_SHADER) CompileShader(GL_FRAGMENT_SHADER, shader_source, shader_id, compilation_successful);

    if (!compilation_successful) return;

    glAttachShader(id, shader_id);
    glLinkProgram(id);
    glValidateProgram(id);

    int validation;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &validation);

    glDetachShader(id, shader_id);

    if (validation == GL_FALSE)
    {
        int length{};
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(id, length, &length, message.get());
        PRINT_LOG(SHADER_TYPE(type) << ": Shader program failed validation.");
        PRINT_LOG(message);
    }
    else
    {
        PRINT_LOG(SHADER_TYPE(type) << ": Shader program passed validation.");
    }
}

void ShaderProgram::SetUniformValue4f(std::string name, float v0, float v1, float v2, float v3)
{
    int location = glGetUniformLocation(id, name.c_str());

    if (location < 0)
    {
        PRINT_LOG("Uniform: \"" << name << "\" invalid.");
        return;
    }

    glUniform4f(location, v0, v1, v2, v3);
}

std::string ShaderProgram::ParseShader(const std::string& file_path)
{
    {
        std::stringstream ss;

        std::fstream stream(file_path);

        if (stream.fail()) throw std::invalid_argument("Fail to open: " + file_path);

        std::string line;
        while (!stream.eof())
        {
            std::getline(stream, line);
            ss << line << '\n';
        }

        return ss.str();
    }
}

void ShaderProgram::CompileShader(GLenum type, const std::string& source, unsigned int& id, bool& validation)
{
    id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(id, length, &length, message.get());
        PRINT_LOG("Failed to compile: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader.");
        PRINT_LOG(message);

        validation = false;
    }

    validation = true;
}
