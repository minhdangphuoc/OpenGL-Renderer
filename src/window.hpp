#pragma once
#define __WINDOW_HPP__
#ifdef __WINDOW_HPP__


#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "GLRenderer.hpp"
#include "Interface.hpp"

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
    bool windowInit(Interface *interface);
    void render(GLRenderer *program, Interface *interface);
    void clean();


private:
    std::unique_ptr<GLFWwindow, glfwDeleter> window;
};

#endif // __WINDOW_HPP__