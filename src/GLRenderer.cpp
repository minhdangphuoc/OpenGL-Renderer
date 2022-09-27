#define POLYGON_MODE 1

#include "GLRenderer.hpp"
#include "Utilities.h"
#include "Controller.hpp"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>


bool GLRenderer::init()
{
    try
    {
        initMaterial();
        loadShaders();
        loadObjects();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void GLRenderer::clean()
{   
    for (auto & it: Objects)
    {
        it.second->clean();
    }
    
}

bool GLRenderer::initMaterial()
{
    MaterialPresets.insert(std::pair("custom", Material(glm::vec3(0.0215f,0.1745f,0.0215f), glm::vec3(0.07568f,0.61424f,0.07568f), glm::vec3(0.633f,0.727811f,0.633f),0.6f*100.f)));
    MaterialPresets.insert(std::pair("emerald", Material(glm::vec3(0.0215f,0.1745f,0.0215f), glm::vec3(0.07568f,0.61424f,0.07568f), glm::vec3(0.633f,0.727811f,0.633f),0.6f*100.f)));
    MaterialPresets.insert(std::pair("jade", Material(glm::vec3(0.135f,0.2225f,0.1575f), glm::vec3(0.54f,0.89f,0.63f), glm::vec3(0.316228f,0.316228f,0.316228f),0.1f*100.f)));
    MaterialPresets.insert(std::pair("obsidian", Material(glm::vec3(0.05375f,0.05f,0.06625f), glm::vec3(0.18275f,0.17f,0.22525f), glm::vec3(0.332741f,0.328634f,0.346435f),0.3f*100.f)));
    MaterialPresets.insert(std::pair("pearl", Material(glm::vec3(0.25f,0.20725f,0.20725f), glm::vec3(1.0f,0.829f,0.829f), glm::vec3(0.296648f,0.296648f,0.296648f),0.088f*100.f)));
    MaterialPresets.insert(std::pair("ruby", Material(glm::vec3(0.1745f,0.01175f,0.01175f), glm::vec3(0.61424f,0.04136f,0.04136f), glm::vec3(0.727811f,0.626959f,0.626959f),0.6f*100.f)));
    MaterialPresets.insert(std::pair("turquoise", Material(glm::vec3(0.1f,0.18725f,0.1745f), glm::vec3(0.396f,0.74151f,0.69102f), glm::vec3(0.297254f,0.30829f,0.306678f),0.1f*100.f)));
    MaterialPresets.insert(std::pair("brass", Material(glm::vec3(0.329412f,0.223529f,0.027451f), glm::vec3(0.780392f,0.568627f,0.113725f), glm::vec3(0.992157f,0.941176f,0.807843f),0.21794872f*100.f)));
    MaterialPresets.insert(std::pair("bronze", Material(glm::vec3(0.2125f,0.1275f,0.054f), glm::vec3(0.714f,0.4284f,0.18144f), glm::vec3(0.393548f,0.271906f,0.166721f),0.2f*100.f)));
    MaterialPresets.insert(std::pair("chrome", Material(glm::vec3(0.25f,0.25f,0.25f), glm::vec3(0.4f,0.4f,0.4f), glm::vec3(0.774597f,0.774597f,0.774597f),0.6f*100.f)));
    MaterialPresets.insert(std::pair("copper", Material(glm::vec3(0.19125f,0.0735f,0.0225f), glm::vec3(0.7038f,0.27048f,0.0828f), glm::vec3(0.256777f,0.137622f,0.086014f),0.1f*100.f)));
    MaterialPresets.insert(std::pair("gold", Material(glm::vec3(0.24725f,0.1995f,0.0745f), glm::vec3(0.75164f,0.60648f,0.22648f), glm::vec3(0.628281f,0.555802f,0.366065f),0.4f*100.f)));
    MaterialPresets.insert(std::pair("silver", Material(glm::vec3(0.19225f,0.19225f,0.19225f), glm::vec3(0.50754f,0.50754f,0.50754f), glm::vec3(0.508273f,0.508273f,0.508273f),0.4f*100.f)));
    MaterialPresets.insert(std::pair("black plastic", Material(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.01f,0.01f,0.01f), glm::vec3(0.5f,0.5f,0.5f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("cyan plastic", Material(glm::vec3(0.0f,0.1f,0.06f), glm::vec3(0.0f,0.50980392f,0.50980392f), glm::vec3(0.50196078f,0.50196078f,0.50196078f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("green plastic", Material(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.1f,0.35f,0.1f), glm::vec3(0.45f,0.55f,0.45f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("red plastic", Material(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.0f,0.0f), glm::vec3(0.7f,0.6f,0.6f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("white plastic", Material(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.55f,0.55f,0.55f), glm::vec3(0.7f,0.7f,0.7f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("yellow plastic", Material(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.0f), glm::vec3(0.6f,0.6f,0.5f),0.25f*100.f)));
    MaterialPresets.insert(std::pair("black rubber", Material(glm::vec3(0.02f,0.02f,0.02f), glm::vec3(0.01f,0.01f,0.01f), glm::vec3(0.4f,0.4f,0.4f),0.078125f*100.f)));
    MaterialPresets.insert(std::pair("cyan rubber", Material(glm::vec3(0.0f,0.05f,0.05f), glm::vec3(0.4f,0.5f,0.5f), glm::vec3(0.04f,0.7f,0.7f),0.078125f*100.f)));
    MaterialPresets.insert(std::pair("green rubber", Material(glm::vec3(0.0f,0.05f,0.0f), glm::vec3(0.4f,0.5f,0.4f), glm::vec3(0.04f,0.7f,0.04f),0.078125f*100.f)));
    MaterialPresets.insert(std::pair("red rubber", Material(glm::vec3(0.05f,0.0f,0.0f), glm::vec3(0.5f,0.4f,0.4f), glm::vec3(0.7f,0.04f,0.04f),0.078125f*100.f)));
    MaterialPresets.insert(std::pair("white rubber", Material(glm::vec3(0.05f,0.05f,0.05f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.7f,0.7f,0.7f),0.078125f*100.f)));
    MaterialPresets.insert(std::pair("yellow rubber", Material(glm::vec3(0.05f,0.05f,0.0f), glm::vec3(0.5f,0.5f,0.4f), glm::vec3(0.7f,0.7f,0.04f),0.078125f*100.f)));

    return true;
}

void GLRenderer::loadShaders()
{
    shaders.insert(std::pair("modelShader", std::make_unique<Shader>("../shaders/colorModel.vert", "../shaders/colorModel.frag")));
    shaders.insert(std::pair("lightShader", std::make_unique<Shader>("../shaders/light.vert", "../shaders/light.frag")));
    shaders.insert(std::pair("cubeShader", std::make_unique<Shader>("../shaders/Cube.vert", "../shaders/Cube.frag")));
}

void GLRenderer::loadObjects()
{
    glEnable(GL_DEPTH_TEST); // Z-Buffer
    Objects.insert(std::pair("Model", std::make_unique<Model>("../Models/miku/scene.gltf")));
    Objects.insert(std::pair("PL1", std::make_unique<Model>("../Models/lightbulb/scene.gltf")));
    Objects.insert(std::pair("PL2", std::make_unique<Model>("../Models/lightbulb/scene.gltf")));
    Objects.insert(std::pair("Cube", std::make_unique<Box>()));
}

void GLRenderer::setCamera(Camera * newCamera)
{
    camera.reset(newCamera);
}



void GLRenderer::draw() 
{
    // draw in wireframe polygons
    if(polyMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // POLYGON_MODE
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();

    shaders.at("modelShader")->use();
    shaders.at("modelShader")->setVec3("viewPos", camera->Position);
    // Directional light
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.direction"), 0.0f, 5.0f, 0.0f);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);	
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.diffuse"), 0.5f, 1.5f, 0.5); 
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

    // Point light 1
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].position"),  0.f, 5.f, 0.f);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].ambient"), 1.f, 1.f, 1.f);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].diffuse"), 1.f, 1.f, 1.f); 
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].specular"), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].linear"), 0.14);
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[0].quadratic"), 0.07);	
    
    // Point light 2
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].position"),  x, y, z);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].ambient"), 0.5f, 0.5f, 0.5f);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].diffuse"), 0.7f, 0.7f, 0.7f); 
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].specular"), 0.5f, 0.5f, 0.5f);  
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].linear"), 0.14);
    glUniform1f(glGetUniformLocation(shaders.at("modelShader")->ID, "pointLights[1].quadratic"), 0.07);	
    
    shaders.at("modelShader")->setMat4("projection", projection);
    shaders.at("modelShader")->setMat4("view", view);

    // render the loaded model
    Objects.at("Model")->model = glm::mat4(1.0f);
    Objects.at("Model")->model = glm::translate(Objects.at("Model")->model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    Objects.at("Model")->model = glm::scale(Objects.at("Model")->model, glm::vec3(1.0f));
    Objects.at("Model")->draw(shaders.at("modelShader").get());
    
    // CUBE
    shaders.at("cubeShader")->use();
    shaders.at("cubeShader")->setVec3("viewPos", camera->Position);
    shaders.at("cubeShader")->setMat4("projection", projection);
    shaders.at("cubeShader")->setMat4("view", view);

    // Directional light
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.direction"), 0.0f, 5.0f, 0.0f);		
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);	
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.diffuse"), 0.5f, 1.5f, 0.5); 
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

    // Point light 1
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].position"),  0.f, 5.f, 0.f);		
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].ambient"), 1.f, 1.f, 1.f);		
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].diffuse"), 1.f, 1.f, 1.f); 
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].specular"), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].linear"), 0.14);
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].quadsratic"), 0.07);	
    
    // Point light 2
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].position"),  x, y, z);		
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].ambient"), 0.5f, 0.5f, 0.5f);		
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].diffuse"), 0.7f, 0.7f, 0.7f); 
    glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].specular"), 0.5f, 0.5f, 0.5f);  
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].linear"), 0.14);
    glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].quadratic"), 0.07);	

    Objects.at("Cube")->model = glm::mat4(1.0f);
    Objects.at("Cube")->model = glm::translate(Objects.at("Cube")->model, glm::vec3(0.f, -.5f, 0.f)); 
    Objects.at("Cube")->model = glm::scale(Objects.at("Cube")->model, glm::vec3(5.f, 1.f, 5.f));
    Objects.at("Cube")->draw(shaders.at("cubeShader").get());

    shaders.at("lightShader")->use();
    shaders.at("lightShader")->setMat4("projection", projection);
    shaders.at("lightShader")->setMat4("view", view);
    
    //PL1
    Objects.at("PL1")->model = glm::mat4(1.0f);
    Objects.at("PL1")->model = glm::translate(Objects.at("PL1")->model, glm::vec3(0, 5, 0));
    Objects.at("PL1")->model = glm::scale(Objects.at("PL1")->model, glm::vec3(0.1f));
    Objects.at("PL1")->draw(shaders.at("lightShader").get());
    
    //PL2
    Objects.at("PL2")->model = glm::mat4(1.0f);
    Objects.at("PL2")->model = glm::translate(Objects.at("PL2")->model, glm::vec3(x, y, z));
    Objects.at("PL2")->model = glm::scale(Objects.at("PL2")->model, glm::vec3(0.1f));
    Objects.at("PL2")->draw(shaders.at("lightShader").get());
}