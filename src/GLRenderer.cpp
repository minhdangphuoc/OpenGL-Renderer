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
        loadTextures();
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
    textures.push_back(std::make_unique<Texture>("../textures/container_darker.png"));
    textures.push_back(std::make_unique<Texture>("../textures/container_specular.png"));
    textures.push_back(std::make_unique<Texture>("../textures/matrix.jpg"));
    
    shaders.at("cubeShader")->use();

    shaders.at("cubeShader")->setInt("material.diffuseMap", 0);
    shaders.at("cubeShader")->setInt("material.specularMap", 1);
    shaders.at("cubeShader")->setInt("material.emissionMap", 2);
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
    glm::vec3 lightPos(x, y, z);
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glm::vec3 pointLightColors[] = {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.3f, 0.1f, 0.1f)
    };
    // draw in wireframe polygons
    if(polyMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // POLYGON_MODE
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->getTexture());
    }
    
    // Changing position of lighting point
    shaders.at("cubeShader")->use();
    shaders.at("cubeShader")->setFloat("time", glfwGetTime());
    shaders.at("cubeShader")->setVec3("viewPos", camera->Position);
    shaders.at("cubeShader")->setFloat("material.shininess", Objects.at("colorCube")->material.shininess);

    // Directional light
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);	
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.diffuse"), 0.05f, 0.05f, 0.05); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
// Point light 1
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].diffuse"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].specular"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].constant"), 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].linear"), 0.14);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[0].quadratic"), 0.07);		
// Point light 2
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].diffuse"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].specular"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].constant"), 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].linear"), 0.14);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[1].quadratic"), 0.07);		
// Point light 3
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].diffuse"), pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].specular") ,pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].constant"), 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].linear"), 0.22);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[2].quadratic"), 0.20);		
// Point light 4
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);		
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].diffuse"), pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].specular"), pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].constant"), 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].linear"), 0.14);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "pointLights[3].quadratic"), 0.07);		
// SpotLight
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.position"), camera->Position.x, camera->Position.y, camera->Position.z);	
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.direction"), camera->Front.x, camera->Front.y, camera->Front.z);
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);	
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.diffuse"), lightColor[0], lightColor[1], lightColor[2]); 
glUniform3f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.constant"), 1.0f);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.linear"), 0.09);
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.quadratic"), 0.032);			
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
glUniform1f(glGetUniformLocation(shaders.at("cubeShader")->ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));     


    // shaders.at("cubeShader")->setVec3("light.position", lightPos);
    // shaders.at("cubeShader")->setVec3("light.position",  camera->Position);
    // shaders.at("cubeShader")->setVec3("light.direction", camera->Front);
    // shaders.at("cubeShader")->setFloat("light.cutOff",   glm::cos(glm::radians(12.5f)));
    // shaders.at("cubeShader")->setInt("isLightMap", 1);

    // light properties
    // shaders.at("cubeShader")->setVec3("light.ambient",  glm::vec3(lightColor.at(0), lightColor.at(1), lightColor.at(2))  * glm::vec3(0.1f));
    // shaders.at("cubeShader")->setVec3("light.diffuse",  glm::vec3(lightColor.at(0), lightColor.at(1), lightColor.at(2)));
    // shaders.at("cubeShader")->setVec3("light.ambient",  0.1f, 0.1f, 0.1f);
    // shaders.at("cubeShader")->setVec3("light.diffuse",  0.8f, 0.8f, 0.8f);
    // shaders.at("cubeShader")->setVec3("light.specular", 1.f, 1.f, 1.f);
    // shaders.at("cubeShader")->setFloat("light.constant",  1.0f);
    // shaders.at("cubeShader")->setFloat("light.linear",    0.09f);
    // shaders.at("cubeShader")->setFloat("light.quadratic", 0.032f);	 
    // shaders.at("cubeShader")->setVec3("light.diffuse",  1.f, 1.f, 1.f);
    // shaders.at("cubeShader")->setVec3("light.specular", 1.f, 1.f, 1.f);

    // shaders.at("cubeShader")->setVec3("material.ambient", glm::vec3(
    //         Objects.at("colorCube")->material.ambient.x, 
    //         Objects.at("colorCube")->material.ambient.y, 
    //         Objects.at("colorCube")->material.ambient.z
    //     ));
            
    // shaders.at("cubeShader")->setVec3("material.diffuse", glm::vec3(
    //         Objects.at("colorCube")->material.diffuse.x, 
    //         Objects.at("colorCube")->material.diffuse.y, 
    //         Objects.at("colorCube")->material.diffuse.z
    //     ));
    // shaders.at("cubeShader")->setVec3("material.specular", glm::vec3(
    //         Objects.at("colorCube")->material.specular.x, 
    //         Objects.at("colorCube")->material.specular.y, 
    //         Objects.at("colorCube")->material.specular.z
    //     ));
    
    // camera/view
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float) wWidth/ (float) wHeight, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    shaders.at("cubeShader")->setMat4("projection", projection);
    shaders.at("cubeShader")->setMat4("view", view);

    // Render Box
    glm::mat4 model = glm::mat4(1.0f); 
    for(unsigned int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f); 
        model = glm::translate(model, glm::vec3(glm::sin(20.f * i/6.f), glm::cos(20.f * i/6.f), i)) + glm::translate(model, glm::vec3(x,y,z));
        
        model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.f, .0f, .0f));
        model = glm::rotate(model, glm::radians(rotY), glm::vec3(.0f, 1.f, .0f));
        model = glm::rotate(model, glm::radians(rotZ), glm::vec3(.0f, .0f, 1.f));
        model = glm::scale(model,  glm::vec3(sX, sY, sZ)); 
        shaders.at("cubeShader")->setMat4("model", model);
          
        glBindVertexArray(Objects.at("colorCube")->shape.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    shaders.at("lightCubeShader")->use();
    shaders.at("lightCubeShader")->setMat4("projection", projection);
    shaders.at("lightCubeShader")->setMat4("view", view);
    // we now draw as many light bulbs as we have point lights.
    glBindVertexArray(Objects.at("lightCube")->shape.VAO);
    for (unsigned int i = 0; i < 4; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        shaders.at("lightCubeShader")->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


}