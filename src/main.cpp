#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

namespace Window
{
    struct glfwDeleter
    {
        void operator()(GLFWwindow *window)
        {
            std::cout << "Destroying GLFW Window Context" << std::endl;
            glfwDestroyWindow(window);
        }
    };
    
    std::unique_ptr<GLFWwindow, glfwDeleter> window;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }  

    void processInput(GLFWwindow *window)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    
    bool GLFWInit()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

        return true;
    }

    bool windowInit()
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

    bool GLADInit()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return false;
        }    
        return true;
    }


    void render()
    {
        while(!glfwWindowShouldClose(window.get()))
        {
            processInput(window.get());
            glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window.get());
            glfwPollEvents();    
        }
    }
}

int main(int argc, const char** argv) 
{
    
    if(!Window::GLFWInit())
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    if(!Window::windowInit())
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    if(!Window::GLADInit())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Window::render();

    glfwTerminate();
    return 0;
}