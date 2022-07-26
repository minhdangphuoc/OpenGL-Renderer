#include "window.hpp"
#include "GLRenderer.hpp"

#include <iostream>


int main(int argc, const char** argv) 
{
    Window window;
    GLRenderer glRenderrer;

    if(!window.GLFWInit())
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    if(!window.windowInit())
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return EXIT_FAILURE;
    }

    if(!window.GLADInit())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    if(glRenderrer.init() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    window.render(glRenderrer);
    
    glRenderrer.clean();
    window.clean();
    
    return 0;
}