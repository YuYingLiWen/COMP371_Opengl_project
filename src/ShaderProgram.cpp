#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <memory>

ShaderProgram::ShaderProgram()
{
	program_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program_id);
}

void ShaderProgram::Bind() const
{
    if (valid) glUseProgram(program_id);
    else PRINT_LOG(name << " is invalid, please fix before use.");
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

void ShaderProgram::Attach(const std::string& vs, const std::string& fs)
{
    name = vs;

    std::string vs_source= ParseShader(vs);
    std::string fs_source = ParseShader(fs);
    
    bool vs_valid = CompileShader(GL_VERTEX_SHADER, vs_source, vs_id);
    bool fs_valid = CompileShader(GL_FRAGMENT_SHADER, fs_source, fs_id);
    
    PRINT_LOG(name);
    PRINT_LOG("Vertex Shader: "   << (vs_valid ? "succesfully compiled." : "failed to compile."));
    PRINT_LOG("Fragment Shader: " << (fs_valid ? "succesfully compiled." : "failed to compile."));

    if (!(vs_valid && fs_valid)) 
    {
        valid = false;
        return;
    }

    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);

    LinkAndValidate();
}

void ShaderProgram::LinkAndValidate()
{
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    int validation;
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &validation);

    glDetachShader(program_id, vs_id);
    glDetachShader(program_id, fs_id);


    if (validation == GL_FALSE)
    {
        valid = false;
        int length{};
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(program_id, length, &length, message.get());
        PRINT_LOG(name << " shader program failed validation.");
        PRINT_LOG(message);
    }
    else
    {
        valid = true;
        PRINT_LOG(name << " shader program passed validation");
    }
}

void ShaderProgram::SetUniformInt(const std::string& name, int integer)
{
    int location = glGetUniformLocation(program_id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform1i(location, integer);
    }
}


void ShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = glGetUniformLocation(program_id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform4f(location, v0, v1, v2, v3);
    }
}

void ShaderProgram::SetUniform4f(const std::string& name, glm::vec4 v4)
{
    int location = glGetUniformLocation(program_id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform4f(location, v4.x, v4.y, v4.z, v4.w);
    }
}

void ShaderProgram::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    int location = glGetUniformLocation(program_id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform3f(location, v0, v1, v2);
    }
}

void ShaderProgram::SetUniform3f(const std::string& name, glm::vec3 v3)
{
    int location = glGetUniformLocation(program_id, name.c_str());

    if (GetUniformLocation(name, location))
    {
        glUniform3f(location, v3.x, v3.y, v3.z);
    }
}

void ShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int location;

    if (GetUniformLocation(name, location))
    {
       glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }
}



bool ShaderProgram::IsValid() const
{
    return valid;
}

bool ShaderProgram::GetUniformLocation(const std::string& name, int& location)
{
    int loc = glGetUniformLocation(program_id, name.c_str());

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

bool ShaderProgram::CompileShader(GLenum type, const std::string& source, unsigned int& shader_id)
{
    shader_id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(shader_id, length, &length, message.get());
        PRINT_LOG("Failed to compile: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader.");
        PRINT_LOG(message);

        return false;
    }

    return true;
}
