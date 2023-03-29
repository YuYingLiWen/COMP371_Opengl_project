#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::Set(glm::vec3 position, glm::vec3 rotation)
{
    Set(position, rotation, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Transform::Set(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    SetPosition(position);
    SetScale(scale);
    SetRotation(rotation);
}

glm::vec3 Transform::Position() const { return position; }

glm::vec3 Transform::Rotation() const { return rotation; }

glm::vec3 Transform::Scale() const { return scale; }

void Transform::SetPosition(glm::vec3 position) { this->position = position; }
void Transform::Translate(glm::vec3 translation) { this->position += translation; }

void Transform::SetRotation(glm::vec3 axis)
{
    rotation = axis;
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
    if (axis.x > 0)
    {
        rotation.x = glm::clamp(rotation.x + angle, -90.0f, 90.0f);
    }
    if (axis.y > 0)
    {
        rotation.y += angle;
    }
    if (axis.z > 0)
    {
        rotation.z += angle;
    }
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;
}

glm::mat4 Transform::LookAt() const
{
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Transform::LookAt(glm::vec3 lookat_pos) const
{
    return glm::lookAt(position, position + (position - lookat_pos), up);
}

glm::vec3 Transform::Right() const { return glm::normalize(glm::cross(up, forward)); }

glm::vec3 Transform::Forward() const { return forward; }
glm::vec3 Transform::Up() const { return up; }

void Transform::SetForward(glm::vec3 forward)
{
    this->forward = forward;
}

void Transform::SetUp(glm::vec3 up)
{
    this->up = up;
}

