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
    // for (auto i: Objects)
    // {
    //     glDeleteVertexArrays(1, &(i.VAO));
    //     glDeleteBuffers(1, &(i.VBO));
    //     glDeleteBuffers(1, &(i.EBO));
    // }
    
}

void GLRenderer::loadShaders()
{
    shaders.insert(std::pair("cubeShader", std::make_unique<Shader>("../shaders/color.vert", "../shaders/color.frag")));
    shaders.insert(std::pair("lightCubeShader", std::make_unique<Shader>("../shaders/light_cube.vert", "../shaders/light_cube.frag")));
}

void GLRenderer::loadObjects()
{
    Objects.insert(std::pair("colorCube", std::make_unique<Object>(Box())));
    Objects.insert(std::pair("lightCube", std::make_unique<Object>(Box())));

    glEnable(GL_DEPTH_TEST); // Z-Buffer

    glGenVertexArrays(1, &(Objects.at("colorCube")->VAO));
    glGenBuffers(1, &(Objects.at("colorCube")->VBO));
    glGenBuffers(1, &(Objects.at("colorCube")->EBO));

    // bind the Vertex Array Object - VAO
    glBindVertexArray(Objects.at("colorCube")->VAO);

    // bind and set vertex buffer(s) - VBO
    glBindBuffer(GL_ARRAY_BUFFER, Objects.at("colorCube")->VBO);
    // Note: "Add i.vertices.size()" * sizeof(GLfloat) when using vector.
    glBufferData(GL_ARRAY_BUFFER, Objects.at("colorCube")->shape.vertices.size() * sizeof(GLfloat), (Objects.at("colorCube")->shape.vertices.data()), GL_STATIC_DRAW);

    // Index Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Objects.at("colorCube")->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Objects.at("colorCube")->shape.indices.size() * sizeof(uint32_t), (Objects.at("colorCube")->shape.indices.data()), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Light Cube
    glGenVertexArrays(1, &(Objects.at("lightCube")->VAO));
    glBindVertexArray(Objects.at("lightCube")->VAO);

    // Vertex Binding
    glBindBuffer(GL_ARRAY_BUFFER, Objects.at("lightCube")->VBO);
   
    // Index Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Objects.at("lightCube")->EBO);
    
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3,  GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    
    shaders.at("cubeShader")->use();
    shaders.at("cubeShader")->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shaders.at("cubeShader")->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
    shaders.at("cubeShader")->setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));

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
    
    
    glBindVertexArray(Objects.at("colorCube")->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // also draw the lamp object
    shaders.at("lightCubeShader")->use();
    shaders.at("lightCubeShader")->setMat4("projection", projection);
    shaders.at("lightCubeShader")->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    shaders.at("lightCubeShader")->setMat4("model", model);

    glBindVertexArray(Objects.at("lightCube")->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}