#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

extern glm::mat4 view;
static bool wiremesh_toggle = false;

static bool W_IS_HELD = false;
static bool A_IS_HELD = false;
static bool S_IS_HELD = false;
static bool D_IS_HELD = false;
static bool Q_IS_HELD = false;
static bool E_IS_HELD = false;


static float last_frame = 0.0f;
static float curr_frame = 0.0f;

static float delta_time = 0.0f;

static float key_speed = 5.0f;
static float rot_degree = 5.0f;
static float rot_speed = 10.0f;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); // DEBUG - TO TEST DEBUG CALLBACK 

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (wiremesh_toggle) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        wiremesh_toggle = !wiremesh_toggle;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)W_IS_HELD = true;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)W_IS_HELD = false;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)A_IS_HELD = true;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)A_IS_HELD = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)S_IS_HELD = true;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)S_IS_HELD = false;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)D_IS_HELD = true;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)D_IS_HELD = false;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)Q_IS_HELD = true;
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)Q_IS_HELD = false;
    
    if (key == GLFW_KEY_E && action == GLFW_PRESS)E_IS_HELD = true;
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)E_IS_HELD = false;
}


void UserInputs()
{
    curr_frame = glfwGetTime();
    delta_time = curr_frame - last_frame;

    if (W_IS_HELD) view = glm::translate(view, glm::vec3(0.0f, -1.0f, 0.0f) * key_speed * delta_time);
    if (A_IS_HELD) view = glm::translate(view, glm::vec3(1.0f, 0.0f, 0.0f) * key_speed * delta_time);
    if (S_IS_HELD) view = glm::translate(view, glm::vec3(0.0f, 1.0f, 0.0f) * key_speed * delta_time);
    if (D_IS_HELD) view = glm::translate(view, glm::vec3(-1.0f, 0.0f, 0.0f) * key_speed * delta_time);

    if (Q_IS_HELD) view = glm::rotate(view, glm::radians(rot_degree) * rot_speed * delta_time, glm::vec3(0.0f, -1.0f, 0.0f));
    if (E_IS_HELD) view = glm::rotate(view, glm::radians(rot_degree) * rot_speed * delta_time, glm::vec3(0.0f, 1.0f, 0.0f));
    
    last_frame = curr_frame;
}