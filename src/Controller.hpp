#pragma once
#ifndef __Controller_hpp__
#define __Controller_hpp__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "CameraSystem.hpp"
#include "GLRenderer.hpp"


class Controller
{
public:
    Controller() = default;
    ~Controller();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

    void setCamera(Camera * newCamera);

    private:
};

#endif //__Controller_hpp__