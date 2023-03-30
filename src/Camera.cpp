#include "Camera.h"

namespace AppTime { extern float DeltaTime(); }

static bool W_IS_HELD = false;
static bool A_IS_HELD = false;
static bool S_IS_HELD = false;
static bool D_IS_HELD = false;
static bool Q_IS_HELD = false;
static bool E_IS_HELD = false;

bool WIREMESH_TOGGLE = false;

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
	height       (780),
	width        (1280),
	aspect_ratio ((float)width / (float)height)
{
    transform.Set(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_ROTATION);
    transform.SetForward(glm::vec3(0.0f, 0.0f, -1.0f));
    transform.SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = transform.LookAt();
}

Camera::Camera(unsigned int height, unsigned int width, float fov)
	: 
    height       (height), 
    width        (width), 
    fov          (fov), 
    aspect_ratio ((float)width / (float)height)
{
    transform.Set(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_ROTATION);
    transform.SetForward(glm::vec3(0.0f, 0.0f, -1.0f));
    transform.SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = transform.LookAt();
}


Camera::~Camera()
{

}

float Camera::FOV() const { return fov; }


float Camera::AspectRatio() const { return aspect_ratio; }

glm::mat4 Camera::GetView()
{
    matrix = transform.LookAt();
    matrix = glm::rotate(matrix, glm::radians(transform.Rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(transform.Rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(transform.Rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

    return matrix;
}

glm::mat4 Camera::GetProjection() const
{
	return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, far_plane);
}

void Camera::ResetCamera(Camera& camera)
{
    camera.transform.SetRotation(DEFAULT_CAMERA_ROTATION);
    camera.transform.SetPosition(DEFAULT_CAMERA_POSITION);
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
        if (WIREMESH_TOGGLE) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        WIREMESH_TOGGLE = !WIREMESH_TOGGLE;
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
    camera.transform.Translate(glm::vec3(0.0f, 0.0f, -yoffset * scroll_speed * AppTime::DeltaTime()));
}

float& Camera::GetKeySpeed()
{
    return key_speed;
}


void Camera::UserInputs(GLFWwindow* window)
{
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    //PRINT_LOG("Mouse Position (x:" << mouse_pos.x << ", y:" << mouse_pos.y << ")");

    if (W_IS_HELD) transform.Translate(transform.Up() * key_speed * AppTime::DeltaTime());
    if (A_IS_HELD) transform.Translate(transform.Right() * key_speed * AppTime::DeltaTime());
    if (S_IS_HELD) transform.Translate(-transform.Up() * key_speed * AppTime::DeltaTime());
    if (D_IS_HELD) transform.Translate(-transform.Right() * key_speed * AppTime::DeltaTime());

    if (Q_IS_HELD)
    {
        camera.transform.Rotate(-rot_degree * rot_speed * AppTime::DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (E_IS_HELD)
    {
        camera.transform.Rotate(rot_degree * rot_speed * AppTime::DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

