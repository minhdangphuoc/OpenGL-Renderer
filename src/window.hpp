#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "GLRenderer.hpp"

class Window
{
public:
    Window() = default;
    ~Window() = default;

    struct glfwDeleter
    {
        void operator()(GLFWwindow *window)
        {
            std::cout << "Destroying GLFW Window Context" << std::endl;
            glfwDestroyWindow(window);
        }
    };

    // Renderer Functions
    bool GLFWInit();
    bool GLADInit();
    bool windowInit();
    void render(GLRenderer * program);
    void clean();


private:
    std::unique_ptr<GLFWwindow, glfwDeleter> window;
};
