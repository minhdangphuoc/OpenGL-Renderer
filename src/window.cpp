#include "window.hpp"
void Window::processInput(GLRenderer *renderer)
{
    ImGuiIO& io = ImGui::GetIO();
    if(io.KeysDown[ImGuiKey_Escape])
        glfwSetWindowShouldClose(window.get(), true);
    
    if (io.KeysDown[ImGuiKey_LeftAlt]) 
    {

        if (isPressAlt == false) isPressAlt = true;
    } else if (isPressAlt == true)
    {
        isPressAlt = false;
        HWInput->firstMouse = true;
        glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }


    if(isPressAlt){
        if (io.KeysDown[ImGuiKey_W])
            renderer->camera->ProcessKeyboard(FORWARD, renderer->deltaTime);
        if (io.KeysDown[ImGuiKey_S])
            renderer->camera->ProcessKeyboard(BACKWARD, renderer->deltaTime);
        if (io.KeysDown[ImGuiKey_A])
            renderer->camera->ProcessKeyboard(LEFT, renderer->deltaTime);
        if (io.KeysDown[ImGuiKey_D])
            renderer->camera->ProcessKeyboard(RIGHT, renderer->deltaTime);

        
        // Mouse pos
        float xpos = static_cast<float>(io.MousePos.x);
        float ypos = static_cast<float>(io.MousePos.y);

        if (HWInput->firstMouse)
        {
            HWInput->lastX = xpos;
            HWInput->lastY = ypos;
            HWInput->firstMouse = false;
        }

        float xoffset = xpos - HWInput->lastX;
        float yoffset = HWInput->lastY - ypos; // reversed since y-coordinates go from bottom to top

        HWInput->lastX = xpos;
        HWInput->lastY = ypos;

        renderer->camera->ProcessMouseMovement(xoffset, yoffset);

        renderer->camera->ProcessMouseScroll(io.MouseWheel);
        
        // std::cerr<< std::to_string(glfwGetTime()) << " " << std::to_string(io.MouseWheel) << std::endl;

        glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);   
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
    window.reset(glfwCreateWindow(1080, 720, "OpenGL", NULL, NULL));

    if (!window)
    {
        return false;
    }


    glfwMakeContextCurrent(window.get());
    interface->init("#version 330", window.get());
    glfwSetFramebufferSizeCallback(window.get(), HWInput->framebuffer_size_callback);  
    
    return true;
}

bool Window::controlInit()
{
    HWInput.reset(new Controller);
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
    std::vector<float> fps, time;
    int max_values = 0;

    while(!glfwWindowShouldClose(window.get()))
    {
        double currentFrame = static_cast<float>(glfwGetTime());
        renderer->deltaTime = currentFrame - renderer->lastFrame;
        renderer->lastFrame = currentFrame;

        processInput(renderer);
        glfwPollEvents();   
        
        
        interface->start();

        interface->beginWindow("Box");
        ImGui::Checkbox("POLYGON_MODE", &(renderer->polyMode));
        interface->createSlider("X", renderer->x, -5.0f, 5.0f);
        interface->createSlider("Y", renderer->y, -5.0f, 5.0f);
        interface->createSlider("Z", renderer->z, -5.0f, 5.0f);
        interface->createSlider("Deg", renderer->deg, .0f, 360.0f);
        interface->createSlider("RotX", renderer->rotX, .0f, 10.0f);
        interface->createSlider("RotY", renderer->rotY, .0f, 10.0f);
        interface->createSlider("RotZ", renderer->rotZ, .0f, 10.0f);
        interface->endWindow();



        interface->beginWindow("Frame");
        ImGui::Text("Application average %.3f ms/frame (%.3f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (max_values + 1 >= 1000)
        {
            fps.erase(fps.begin());
        } else 
        {
            max_values++;
            time.push_back(max_values);
        }
        fps.push_back(ImGui::GetIO().Framerate);

        interface->createPlotLine("fps", time, fps, max_values);
        interface->endWindow();

        interface->render();

        renderer->draw();
        
        interface->renderDrawData();
        
        glfwSwapBuffers(window.get());
    }
}

void Window::clean()
{
    // glfwDestroyWindow(window.get());
    glfwTerminate();
}