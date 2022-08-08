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
    
    if (glfwGetKey(window.get(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS) 
    {

        if (isPressAlt == false) isPressAlt = true;
    } else if (isPressAlt == true)
    {
        isPressAlt = false;
    }


    if(isPressAlt){

        glfwSetCursorPosCallback(window.get(), HWInput->mouse_callback);
        glfwSetScrollCallback(window.get(), HWInput->scroll_callback);
        glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);   
    }
    else {
        glfwSetCursorPosCallback(window.get(), nullptr);
        glfwSetScrollCallback(window.get(), nullptr);
        glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
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
    HWInput.reset(new Controller);
    HWInput->setCamera(camera.get());
      
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

uint32_t Window::framePerSecond(){
    uint32_t fps;
    // per-frame time logic
    // --------------------
    double currentFrame = static_cast<float>(glfwGetTime());
    renderer->deltaTime = currentFrame - renderer->lastFrame;
    renderer->lastFrame = currentFrame;

    
    frameCount++;

    if (currentFrame - renderer->previousFrame >= 1.f)
    {
        fps = frameCount;
        frameCount = 0;
        renderer->previousFrame = currentFrame;
        return fps;   
    }
    return 0;
}

void Window::render(GLRenderer *renderer, Interface *interface)
{
    this -> renderer.reset(renderer);
    renderer->previousFrame = glfwGetTime();
    while(!glfwWindowShouldClose(window.get()))
    {
        int fps = framePerSecond();
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

        interface->beginWindow("Frame");
        interface->createText(std::to_string(fps));
        interface->endWindow();

        interface->render();

        renderer->draw();
        
        interface->renderDrawData();
        
        glfwSwapBuffers(window.get());
        glfwPollEvents();   
    }
}

void Window::clean()
{
    // glfwDestroyWindow(window.get());
    glfwTerminate();
}