#include "Interface.hpp"


void Interface::init(const std::string & glsl_version, GLFWwindow * window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::Spectrum::StyleColorsSpectrum();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    io.ConfigInputTrickleEventQueue == false;
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void Interface::start()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame(); 
}

void Interface::render()
{
    ImGui::Render();
}

void Interface::renderDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Interface::clean()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void Interface::beginWindow(std::string wName, bool & isActivate)
{
    if (isActivate)
    ImGui::Begin(wName.c_str(), &isActivate);
}

void Interface::beginWindow(std::string wName)
{
    ImGui::Begin(wName.c_str());
}

void Interface::endWindow()
{
    ImGui::End();
}

void Interface::createText(std::string title)
{  
    ImGui::Text(title.c_str());
}

void Interface::createSlider(std::string title, float &f, const float min, const float max)
{
    ImGui::SliderFloat(title.c_str(), &f, min, max);
}

void Interface::createPlotLine(std::string title, std::vector<float> & x_data, std::vector<float> & y_data, int size)
{
    ImPlot::BeginPlot("FPS Line Chart", ImVec2(320.f, 240.f));
    ImPlot::PlotLine(title.c_str(), x_data.data(), y_data.data(), size);
    ImPlot::EndPlot();
}

void Interface::createColorEdit3(std::string title, std::array<float, 3>  & color)
{
    ImGui::ColorEdit3(title.c_str(), color.data());
}