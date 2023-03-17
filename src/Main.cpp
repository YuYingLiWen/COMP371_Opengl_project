

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "InputsHandler.h"

static std::string ParseShader(const std::string& file_path)
{
    std::stringstream ss;
    std::fstream stream(file_path);
    std::string line;
    while (!stream.eof())
    {
        std::getline(stream, line);
        ss << line << '\n';
    }

    return ss.str();
}

static unsigned CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
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
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    int validation;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validation);
    if (validation == GL_FALSE)
    {
        int length{};
        auto message = std::make_unique<char>();
        glGetShaderInfoLog(program, length, &length, message.get());
        PRINT_LOG("Shader program failed validation.");
        PRINT_LOG(message);
    }
    else
    {
        PRINT_LOG("Shader program passed validation.");
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // Set Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) 
    { 
        PRINT_LOG("GLEW INIT ERROR."); 
        return -1;
    }
    else
        PRINT_LOG("GLEW INIT SUCCESSFUL.");

    // User Inputs
    glfwSetKeyCallback(window, KeyCallback);

    // Debug
#ifdef _DEBUG
    EnableDebug();
#endif

    PRINT_LOG(glGetString(GL_VERSION)); // Prints Opengl version

    float positions[8] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f,
    };

    // Creating index buffer
    unsigned int indices[]=
    {
        0, 1, 2,
        2, 0, 3
    };

    VertexArray vao;
    VertexBuffer vbo(positions, sizeof(positions), GL_STATIC_DRAW);
    ElementBuffer ebo(indices, sizeof(indices));
    vao.SetLayout(0, 2, GL_FLOAT, 2);

    /// Shaders
    std::string fs = ParseShader("res\\shaders\\fragment.shader");
    std::string vs = ParseShader("res\\shaders\\vertex.shader");

    unsigned int shader = CreateShader(vs, fs);
    glUseProgram(shader);

    int u_location = glGetUniformLocation(shader, "u_color");
    //u_location 
    glUniform4f(u_location, 0.50f, 0.1f, 0.1f, 1.0f);
    
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f;
    float increment = 0.1f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Resize window
        glfwSetFramebufferSizeCallback(window, WindowSizeCallback);


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        vao.Bind();

        glUniform4f(u_location, r, 0.1f, 0.5f, 1.0f);

        if (r > 1.0f || r < 0.0f) increment = -increment;

        r += increment;

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw as wired frame mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Draw filled
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //Draws with index buffer
        //glDrawArrays(GL_TRIANGLES, 0, 4); //Draws raw without index buffer


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
