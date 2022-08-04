#pragma once
#define __INTERFACE_HPP__
#ifdef __INTERFACE_HPP__

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_spectrum.h"
#include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class Interface
{
private:
    
public:
    // Con/Destructor
    Interface() = default;
    ~Interface() = default;

    void init(const std::string & glsl_version, GLFWwindow * window);
    void render();
    void renderDrawData();
    void clean();
    void start();

    void beginWindow(std::string wName, bool & isActivate);
    void beginWindow(std::string wName);

    void endWindow();

    // Component
    void createText(std::string title);
    void CreateSlider(std::string title, float &f, const float min, const float max);

};

#endif // __INTERFACE_HPP__