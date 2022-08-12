#pragma once
#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_spectrum.h"

#include "imgui/implot.h"
#include "imgui/implot_internal.h"
#include "glm/glm.hpp"

#include <string>
#include <memory>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <array>
#include <vector>
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
    void createColorEdit3(std::string title, float * color);

    // Create Box
    void createComboBox(const std::string title, unsigned int & selected, const std::vector<std::string> & list);
};

#endif // __INTERFACE_HPP__