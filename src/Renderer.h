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
    void Draw(const SceneObject& obj);

    void Draw(GLenum mode, const SceneObject& obj);

    void Clear() const;
};
