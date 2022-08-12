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
        Object::initMaterial();
        // loadTextures();
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
        glDeleteVertexArrays(1, &(it.second->shape.VAO));
        glDeleteBuffers(1, &(it.second->shape.VBO));
        glDeleteBuffers(1, &(it.second->shape.EBO));
    }
    
}

void GLRenderer::loadShaders()
{
    shaders.insert(std::pair("cubeShader", std::make_unique<Shader>("../shaders/color.vert", "../shaders/color.frag")));
    shaders.insert(std::pair("lightCubeShader", std::make_unique<Shader>("../shaders/light_cube.vert", "../shaders/light_cube.frag")));
}

void GLRenderer::loadObjects()
{
    glEnable(GL_DEPTH_TEST); // Z-Buffer

    Objects.insert(std::pair("colorCube", std::make_unique<Object>(Box())));
    Objects.insert(std::pair("lightCube", std::make_unique<Object>(LightCube())));
}

void GLRenderer::loadTextures()
{
    textures.push_back(std::make_unique<Texture>("../textures/wall.jpg"));
    textures.push_back(std::make_unique<Texture>("../textures/pepe.png"));
    
    shaders.at("cubeShader")->use();

    for (int i = 0; i < textures.size(); i++)
    {
        shaders.at("cubeShader")->setInt("texture"+std::to_string(i), i);
    }

    shaders.at("lightCubeShader")->use();

    for (int i = 0; i < textures.size(); i++)
    {
        shaders.at("lightCubeShader")->setInt("texture"+std::to_string(i), i);
    }
}

void GLRenderer::setProjection()
{
    
}

void GLRenderer::setCamera(Camera * newCamera)
{
    camera.reset(newCamera);
}


void GLRenderer::draw() 
{
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    lightPos.x = sin(glfwGetTime()) * 1.5f;
    lightPos.z = cos(glfwGetTime()) * 1.5f;


    // draw in wireframe polygons
    if(polyMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // POLYGON_MODE
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // for (int i = 0; i < textures.size(); i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, textures[i]->getTexture());
    // }

    if (Objects.at("colorCube")->selectedMaterial > 0)
    {
        Objects.at("colorCube")->material = Objects.at("colorCube")->MaterialPresets.at(MaterialNames.at(Objects.at("colorCube")->selectedMaterial));
    }
    
    // Changing position of lighting point
    shaders.at("cubeShader")->use();
    shaders.at("cubeShader")->setVec3("light.position", lightPos);
    shaders.at("cubeShader")->setVec3("viewPos", camera->Position);

    // light properties
    shaders.at("cubeShader")->setVec3("light.diffuse",  glm::vec3(lightColor.at(0), lightColor.at(1), lightColor.at(2)) * glm::vec3(0.5f));
    // shaders.at("cubeShader")->setVec3("light.diffuse",  1.f, 1.f, 1.f);
    shaders.at("cubeShader")->setVec3("light.ambient",  1.f, 1.f, 1.f);
    shaders.at("cubeShader")->setVec3("light.specular", 1.f, 1.f, 1.f);
    // shaders.at("cubeShader")->setVec3("light.ambient",  glm::vec3(lightColor.at(0), lightColor.at(1), lightColor.at(2)) * glm::vec3(0.5f) * glm::vec3(0.2f));
    // shaders.at("cubeShader")->setVec3("light.specular", 1.f, 1.f, 1.f);

    shaders.at("cubeShader")->setVec3("material.ambient", glm::vec3(
            Objects.at("colorCube")->material.ambient.x, 
            Objects.at("colorCube")->material.ambient.y, 
            Objects.at("colorCube")->material.ambient.z
        ));
            
    shaders.at("cubeShader")->setVec3("material.diffuse", glm::vec3(
            Objects.at("colorCube")->material.diffuse.x, 
            Objects.at("colorCube")->material.diffuse.y, 
            Objects.at("colorCube")->material.diffuse.z
        ));
    shaders.at("cubeShader")->setVec3("material.specular", glm::vec3(
            Objects.at("colorCube")->material.specular.x, 
            Objects.at("colorCube")->material.specular.y, 
            Objects.at("colorCube")->material.specular.z
        ));
    shaders.at("cubeShader")->setFloat("material.shininess", Objects.at("colorCube")->material.shininess * 100.f);
    
    // camera/view
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float) wWidth/ (float) wHeight, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shaders.at("cubeShader")->setMat4("projection", projection);
    shaders.at("cubeShader")->setMat4("view", view);

    // Render Box
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.f, .0f, .0f));
    model = glm::rotate(model, glm::radians(rotY), glm::vec3(.0f, 1.f, .0f));
    model = glm::rotate(model, glm::radians(rotZ), glm::vec3(.0f, .0f, 1.f));
    model = glm::scale(model,  glm::vec3(sX, sY, sZ)); 
    shaders.at("cubeShader")->setMat4("model", model);
    
    
    glBindVertexArray(Objects.at("colorCube")->shape.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // also draw the lamp object
    shaders.at("lightCubeShader")->use();
    shaders.at("lightCubeShader")->setMat4("projection", projection);
    shaders.at("lightCubeShader")->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    shaders.at("lightCubeShader")->setMat4("model", model);

    glBindVertexArray(Objects.at("lightCube")->shape.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}