#include "window.hpp"
void Window::processInput()
{
    if(glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.get(), true);

    if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, renderer->deltaTime);
    if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, renderer->deltaTime);
    if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, renderer->deltaTime);
    if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, renderer->deltaTime);
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
    glfwSetFramebufferSizeCallback(window.get(), HWInput->framebuffer_size_callback);  
    interface->init("#version 330", window.get());
    return true;
}

bool Window::controlInit()
{
    auto pCamera = camera.get();
    HWInput.reset(new Controller);
    HWInput->setCamera(pCamera);
    glfwSetCursorPosCallback(window.get(), HWInput->mouse_callback);
    glfwSetScrollCallback(window.get(), HWInput->scroll_callback);
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
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

void Window::setCamera(Camera * newCamera)
{
    camera.reset(newCamera);
}

void Window::render(GLRenderer *renderer, Interface *interface)
{
    this -> renderer.reset(renderer);
    while(!glfwWindowShouldClose(window.get()))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        renderer->deltaTime = currentFrame - renderer->lastFrame;
        renderer->lastFrame = currentFrame;

        processInput();
        
        interface->start();

        interface->beginWindow("Translate");
        interface->CreateSlider("X", renderer->x, -5.0f, 5.0f);
        interface->CreateSlider("Y", renderer->y, -5.0f, 5.0f);
        interface->CreateSlider("Z", renderer->z, -5.0f, 5.0f);
        interface->endWindow();

        interface->beginWindow("Rotate");
        interface->CreateSlider("Deg", renderer->deg, .0f, 360.0f);
        interface->CreateSlider("RotX", renderer->rotX, .0f, 10.0f);
        interface->CreateSlider("RotY", renderer->rotY, .0f, 10.0f);
        interface->CreateSlider("RotZ", renderer->rotZ, .0f, 10.0f);
        interface->endWindow();

        interface->render();

        renderer->draw();
        
        interface->renderDrawData();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
        glfwPollEvents();    
    }
}

void Window::clean()
{
    // glfwDestroyWindow(window.get());
    glfwTerminate();
}