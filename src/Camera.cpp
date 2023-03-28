#include "Camera.h"

namespace AppTime { extern float DeltaTime(); }

static bool W_IS_HELD = false;
static bool A_IS_HELD = false;
static bool S_IS_HELD = false;
static bool D_IS_HELD = false;
static bool Q_IS_HELD = false;
static bool E_IS_HELD = false;

static bool wiremesh_toggle = false;

static float key_speed = 5.0f;
static float rot_degree = 5.0f;
static float rot_speed = 10.0f;

static float scroll_speed = 100.0f;

static glm::dvec2 mouse_pos;



// Camera
extern Camera camera;
#define DEFAULT_CAMERA_ROTATION glm::vec3(35.0f, 45.0f, 0.0f)
#define DEFAULT_CAMERA_POSITION glm::vec3(0.0f, 0.0f, 10.0f)
static float far_plane = 500.0f;




Camera::Camera()
    :
    fov          (65.0f), 
    position     (DEFAULT_CAMERA_POSITION),
    forward      (glm::vec3(0.0f, 0.0f, -1.0f)),
	up           (glm::vec3(0.0f, 1.0f, 0.0f)),
	height       (780),
	width        (1280),
    matrix       (glm::lookAt(position, position + forward, up)),
	aspect_ratio ((float)width / (float)height),
    rotation     (DEFAULT_CAMERA_ROTATION)
{}

Camera::Camera(unsigned int height, unsigned int width, float fov)
	: 
    height       (height), 
    width        (width), 
    fov          (fov), 
    aspect_ratio ((float)width / (float)height),
    position     (DEFAULT_CAMERA_POSITION),
    forward      (glm::vec3(0.0f, 0.0f, -1.0f)),
    up           (glm::vec3(0.0f, 1.0f, 0.0f)),
    matrix       (glm::lookAt(position, position + forward, up)),
    rotation     (DEFAULT_CAMERA_ROTATION)
{}


Camera::~Camera()
{

}

float Camera::FOV() const { return fov; }

glm::vec3 Camera::GetPosition() const { return position; }
void Camera::SetPosition(glm::vec3 position) { this->position = position; }
void Camera::Translate(glm::vec3 translation) { this->position += translation;  }

glm::vec3 Camera::Forward() const { return forward; }
glm::vec3 Camera::Up() const {
    return up;
}// glm::normalize(glm::cross(forward, Right())); }
glm::vec3 Camera::Right() const { return glm::normalize(glm::cross(up, forward)); }
float Camera::AspectRatio() const { return aspect_ratio; }

glm::mat4 Camera::GetView()
{
    matrix = glm::lookAt(position, position + forward, up);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return matrix;
}

glm::mat4 Camera::GetProjection() const
{
	return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, far_plane);
}

glm::mat4 Camera::LookAt(glm::vec3 lookat_pos) const
{
    return glm::lookAt(position, position + (position - lookat_pos), up);
}

void Camera::Rotate(float angle, glm::vec3 axis)
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

void Camera::SetRotation(glm::vec3 axis)
{
    rotation = axis;
}

void Camera::ResetCamera(Camera& camera)
{
    camera.SetRotation(DEFAULT_CAMERA_ROTATION);
    camera.SetPosition(DEFAULT_CAMERA_POSITION);
}

void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {

    }

    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {

    }


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

void Camera::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.Translate(glm::vec3(0.0f, 0.0f, -yoffset * scroll_speed * AppTime::DeltaTime()));
}

float& Camera::GetKeySpeed()
{
    return key_speed;
}


void Camera::UserInputs(GLFWwindow* window)
{
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    PRINT_LOG("Mouse Position (x:" << mouse_pos.x << ", y:" << mouse_pos.y << ")");

    if (W_IS_HELD) this->Translate(Up() * key_speed * AppTime::DeltaTime());
    if (A_IS_HELD) this->Translate(Right() * key_speed * AppTime::DeltaTime());
    if (S_IS_HELD) this->Translate(-Up() * key_speed * AppTime::DeltaTime());
    if (D_IS_HELD) this->Translate(-Right() * key_speed * AppTime::DeltaTime());

    if (Q_IS_HELD) camera.Rotate(rot_degree * rot_speed * AppTime::DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    if (E_IS_HELD) camera.Rotate(-rot_degree * rot_speed * AppTime::DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
}
