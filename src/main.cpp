#include <iostream>

#include "window.hpp"
#include "Interface.hpp"

int main(int argc, const char** argv) 
{
    Window window;
    GLRenderer glRenderrer;
    Interface interface;
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    glRenderrer.setCamera(&camera);
    
    if(!window.GLFWInit())
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    if(!window.controlInit())
    {
        std::cerr << "Failed to initialize controller" << std::endl;
        return EXIT_FAILURE;
    }

    if(!window.windowInit(&interface))
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return EXIT_FAILURE;
    }

    if(!window.GLADInit())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }




    glRenderrer.setShaderPaths("../shaders/shader.vert", "../shaders/shader.frag");

    if(glRenderrer.init() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    window.render(&glRenderrer, &interface);
    
    interface.clean();
    window.clean();
    
    return 0;
}