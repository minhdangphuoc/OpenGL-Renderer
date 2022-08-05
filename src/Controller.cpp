#include "Controller.hpp"

bool firstMouse = true;
float lastX = 800.f/2.f;
float lastY = 600.f/2.f;
std::unique_ptr<Camera> ourCamera;

Controller::~Controller()
    {
        ourCamera.reset();
    }


void Controller::setCamera(Camera * newCamera)
{
    ourCamera.reset(newCamera);
}

void Controller::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void Controller::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    ourCamera->ProcessMouseMovement(xoffset, yoffset);
}

void Controller::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ourCamera->ProcessMouseScroll(static_cast<float>(yoffset));
}