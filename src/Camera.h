#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();
	Camera(unsigned int height, unsigned int width, float fov);

	~Camera();

	float FOV() const;
	float AspectRatio() const;

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);

	void Rotate(float angle, glm::vec3 axis);
	void SetRotation(glm::vec3 axis);

	glm::vec3 Forward() const;
	glm::vec3 Up() const;
	glm::vec3 Right() const;
	glm::mat4 GetView();
	glm::mat4 GetProjection() const;
	
	glm::mat4 LookAt(glm::vec3 postion) const;
	float& GetKeySpeed();

	void UserInputs();

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ResetCamera(Camera&);

public:
	glm::vec3 forward;

	unsigned int height;
	unsigned int width;


private:
	glm::vec3 position;
	glm::vec3 rotation;

	float fov;
	glm::vec3 up;

	float aspect_ratio;

	float key_speed = 5.0f;
	glm::mat4 matrix = glm::mat4(1.0f);
};

