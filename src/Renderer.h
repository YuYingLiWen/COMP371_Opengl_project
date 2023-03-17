#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "ElementBuffer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"

#include "Utils.h"



void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

void EnableDebug();


// Window Callbacks
void WindowSizeCallback(GLFWwindow* window, int width, int height);

enum DrawMode 
{
    FILL, WIREFRAME
};
class Renderer
{
public:
    void Draw(const VertexArray& vao, const ElementBuffer& ebo, const ShaderProgram& shader);
    void SetDrawMode(DrawMode mode) const;
    void Clear() const;
};
