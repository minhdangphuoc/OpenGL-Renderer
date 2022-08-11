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
    glm::vec3 lightPos(0.0f, 2.0f, 0.0f);


    // draw in wireframe polygons
    if(polyMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // POLYGON_MODE
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // for (int i = 0; i < textures.size(); i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, textures[i]->getTexture());
    // }
    
    // Changing position of lighting point
    lightPos.x = sin(glfwGetTime()) * 2.f;
    lightPos.z = cos(glfwGetTime()) * 2.f;

    shaders.at("cubeShader")->use();
    shaders.at("cubeShader")->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shaders.at("cubeShader")->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    shaders.at("cubeShader")->setVec3("lightPos", lightPos);
    shaders.at("cubeShader")->setVec3("viewPos", camera->Position);
    
    // camera/view
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)1080.f/ (float)720.f  , 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shaders.at("cubeShader")->setMat4("projection", projection);
    shaders.at("cubeShader")->setMat4("view", view);

    // Render Box
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(deg), glm::vec3(rotX, rotY, rotZ));
    shaders.at("cubeShader")->setMat4("model", model);
    
    
    glBindVertexArray(Objects.at("colorCube")->shape.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Render smaller Box
    model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(x, y + .75f, z));
    model = glm::rotate(model, glm::radians(deg), glm::vec3(rotX, rotY, rotZ));
    model = glm::scale(model, glm::vec3(0.5f)); // a smaller cube
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