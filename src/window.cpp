#include "window.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


bool Window::GLFWInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    return true;
}

bool Window::windowInit()
{
    window.reset(glfwCreateWindow(800, 600, "OpenGL", NULL, NULL));

    if (!window)
    {
        return false;
    }
    glfwMakeContextCurrent(window.get());
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);  
    return true;
}

bool Window::GLADInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }    
    return true;
}


void Window::render(GLRenderer renderer)
{
    while(!glfwWindowShouldClose(window.get()))
    {
        processInput(window.get());
        
        renderer.draw();

        glfwSwapBuffers(window.get());
        glfwPollEvents();    
    }
}

void Window::clean()
{
    glfwDestroyWindow(window.get());
    glfwTerminate();
}