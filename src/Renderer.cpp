
#include "Renderer.h"

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // Source: https://learnopengl.com/In-Practice/Debugging
    // Removed useless errors.
    // 
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    ERROR_LOG("(" << id << "): " << message);
    __debugbreak();
}


void EnableDebug()
{
    // Must be called after glfw init & glew init.
    // Source: https://learnopengl.com/In-Practice/Debugging

    int flag = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flag);

    if (flag & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        PRINT_LOG("Debug context: enabled.");

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            nullptr,
            GL_TRUE);

    }
    else { PRINT_LOG("Debug context: disabled."); }
}


// Window Callbacks

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::Draw(const SceneObject& obj)
{
    obj.Bind();

    glDrawElements(GL_TRIANGLES, obj.GetCount(), GL_UNSIGNED_INT, nullptr); //Draws with index buffer
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
