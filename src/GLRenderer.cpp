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
        it.second->cleanMesh();
    }
    
}

void GLRenderer::loadShaders()
{
    shaders.insert(std::pair("modelShader", std::make_unique<Shader>("../shaders/colorModel.vert", "../shaders/colorModel.frag")));
    shaders.insert(std::pair("lightShader", std::make_unique<Shader>("../shaders/light.vert", "../shaders/light.frag")));
}

void GLRenderer::loadObjects()
{
    glEnable(GL_DEPTH_TEST); // Z-Buffer
    Objects.insert(std::pair("Guitar", std::make_unique<Model>("../Models/miku/scene.gltf")));
    Objects.insert(std::pair("Light", std::make_unique<Model>("../Models/lightbulb/scene.gltf")));
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
    
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shaders.at("modelShader")->setMat4("projection", projection);
    shaders.at("modelShader")->setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f));
    shaders.at("modelShader")->setMat4("model", model);
    Objects.at("Guitar")->Draw(shaders.at("modelShader").get());
    
    shaders.at("lightShader")->use();
    shaders.at("lightShader")->setMat4("projection", projection);
    shaders.at("lightShader")->setMat4("view", view);
    
    //PL1
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 5, 0));
    model = glm::scale(model, glm::vec3(0.1f));
    shaders.at("lightShader")->setMat4("model", model);
    Objects.at("Light")->Draw(shaders.at("lightShader").get());
    
    //PL2
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::scale(model, glm::vec3(0.1f));
    shaders.at("lightShader")->setMat4("model", model);
    Objects.at("Light")->Draw(shaders.at("lightShader").get());
}