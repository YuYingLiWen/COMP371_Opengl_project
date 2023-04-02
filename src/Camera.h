#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Utils.h"
#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(unsigned int height, unsigned int width, float fov);

	~Camera();

	float FOV() const;
	float AspectRatio() const;

	glm::mat4 GetView();
	glm::mat4 GetProjection() const;
	
	float& GetKeySpeed();

	void UserInputs(GLFWwindow* window);

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ResetCamera(Camera&);

	Transform& GetTransform();

public:

	unsigned int height;
	unsigned int width;


private:

	Transform transform;

	float fov;

	float aspect_ratio;

	float key_speed = 50.0f;
	glm::mat4 matrix = glm::mat4(1.0f);
};

