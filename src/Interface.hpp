#pragma once
#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_spectrum.h"

#include "imgui/implot.h"
#include "imgui/implot_internal.h"

#include <string>
#include <vector>

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
    void createSlider(std::string title, float &f, const float min, const float max);
    void createPlotLine(std::string title, std::vector<float> & x_data, std::vector<float> & y_data, int size);

};

#endif // __INTERFACE_HPP__