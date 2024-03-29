#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"
#include "SceneObject.h"

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


class Renderer
{
public:
    void Draw(SceneObject& obj) const;

    void Draw(GLenum mode,SceneObject& obj) const;

    void Clear() const;
};
