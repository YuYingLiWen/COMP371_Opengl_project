#pragma once

#include <GLFW/glfw3.h>

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); // DEBUG - TO TEST DEBUG CALLBACK 

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
