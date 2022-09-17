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
    shaders.insert(std::pair("modelShader", std::make_unique<Shader>("../shaders/color.vert", "../shaders/color.frag")));
    shaders.insert(std::pair("lightShader", std::make_unique<Shader>("../shaders/light_cube.vert", "../shaders/light_cube.frag")));
}

void GLRenderer::loadObjects()
{
    glEnable(GL_DEPTH_TEST); // Z-Buffer
    Objects.insert(std::pair("Guitar", std::make_unique<Model>("../Models/miku/scene.gltf")));
    Objects.insert(std::pair("light", std::make_unique<Model>("../Models/lightbulb/scene.gltf")));
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

    // Directional light
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.direction"), 1.2f, 1.0f, 2.0f);		
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.diffuse"), 1.f, 1.f, 1.f); 
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.ambient"), 1.f, 1.f, 1.f);	
    glUniform3f(glGetUniformLocation(shaders.at("modelShader")->ID, "dirLight.specular"), 1.f, 1.f, 1.f);


    // don't forget to enable shader before setting uniforms
    shaders.at("modelShader")->use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shaders.at("modelShader")->setMat4("projection", projection);
    shaders.at("modelShader")->setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    shaders.at("modelShader")->setMat4("model", model);
    Objects.at("Guitar")->Draw(shaders.at("modelShader").get());

    // we now draw as many light bulbs as we have point lights.
    shaders.at("lightShader")->use();
    shaders.at("lightShader")->setMat4("projection", projection);
    shaders.at("lightShader")->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 1.f, 1.f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.f,.0f,.0f));
    model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
    shaders.at("lightShader")->setMat4("model", model);
    Objects.at("light")->Draw(shaders.at("lightShader").get());
}