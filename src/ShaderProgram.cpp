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
    delete fs_id;
    delete vs_id;

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

    std::string shader_source = ParseShader(file_path);
    bool valid_shader = false;

    if (type == GL_VERTEX_SHADER) 
    {
        if (valid_shader = CompileShader(GL_VERTEX_SHADER, shader_source, shader_id)) 
            vs_id = new int(shader_id);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        if (valid_shader = CompileShader(GL_FRAGMENT_SHADER, shader_source, shader_id)) 
            fs_id = new int(shader_id);
    }

     PRINT_LOG(SHADER_TYPE(type) << " " << (valid_shader ? "succesfully compiled." : "failed to compile."));


    if (!valid_shader) return;

    glAttachShader(id, shader_id);
}

void ShaderProgram::LinkAndValidate()
{
    glLinkProgram(id);
    glValidateProgram(id);

    int validation;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &validation);

    glDetachShader(id, *vs_id);
    glDetachShader(id, *fs_id);

    if (validation == GL_FALSE)
    {
        int length{};
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(id, length, &length, message.get());
        PRINT_LOG("Shader program failed validation.");
        PRINT_LOG(message);
    }
    else
    {
        PRINT_LOG("Shader program passed validation.");
    }
}

void ShaderProgram::SetUniformValue4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = glGetUniformLocation(id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform4f(location, v0, v1, v2, v3);
    }
}

void ShaderProgram::SetUniformValueMat4f(const std::string& name, const glm::mat4& matrix)
{
    int location;

    if (GetUniformLocation(name, location))
    {
       glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }
}

bool ShaderProgram::GetUniformLocation(const std::string& name, int& location)
{
    int loc = glGetUniformLocation(id, name.c_str());

    if (loc < 0)
    {
        PRINT_LOG("Uniform: \"" << name << "\" not found.");
        return false;
    }

    location = loc;
    return true;
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

bool ShaderProgram::CompileShader(GLenum type, const std::string& source, unsigned int& id)
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

        return false;
    }

    return true;
}
