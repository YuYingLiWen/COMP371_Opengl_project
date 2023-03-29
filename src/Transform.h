#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Utils.h"

class Transform
{
public:
	Transform();
	~Transform();

	void Set(glm::vec3 position, glm::vec3 rotation);
	void Set(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	glm::vec3 Position() const;
	glm::vec3 Rotation() const;
	glm::vec3 Scale() const;

	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);

	void SetRotation(glm::vec3 axis);

	void Rotate(float angle, glm::vec3 axis);

	void SetScale(glm::vec3 scale);

	glm::mat4 LookAt() const;
	glm::mat4 LookAt(glm::vec3 lookat_pos) const;


	glm::vec3 Right() const;
	glm::vec3 Forward() const;
	glm::vec3 Up() const;

	void SetForward(glm::vec3 forward);
	void SetUp(glm::vec3 up);


private:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::vec3 up{};
	glm::vec3 forward{};
};

