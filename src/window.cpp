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
    glfwSwapInterval(1);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    

    return true;
}

bool Window::windowInit(Interface *interface)
{
    window.reset(glfwCreateWindow(800, 600, "OpenGL", NULL, NULL));

    if (!window)
    {
        return false;
    }
    
    glfwMakeContextCurrent(window.get());
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);  
    interface->init("#version 330", window.get());
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


void Window::render(GLRenderer *renderer, Interface *interface)
{
    while(!glfwWindowShouldClose(window.get()))
    {
        processInput(window.get());
        glfwPollEvents();    
        
        interface->start();

        interface->beginWindow("Translate");
        interface->CreateSlider("X", renderer->x, -1.0f, 1.0f);
        interface->CreateSlider("Y", renderer->y, -1.0f, 1.0f);
        interface->CreateSlider("Z", renderer->z, -1.0f, 1.0f);
        interface->endWindow();

        interface->beginWindow("Rotate");
        interface->CreateSlider("Deg", renderer->deg, .0f, 360.0f);
        interface->CreateSlider("RotX", renderer->rotX, .0f, 10.0f);
        interface->CreateSlider("RotY", renderer->rotY, .0f, 10.0f);
        interface->CreateSlider("RotZ", renderer->rotZ, .0f, 10.0f);
        interface->endWindow();

        interface->render();

        renderer->draw();
        renderer->motion();
        
        interface->renderDrawData();

        glfwSwapBuffers(window.get());
    }
}

void Window::clean()
{
    glfwDestroyWindow(window.get());
    glfwTerminate();
}