#define POLYGON_MODE 0

#include "GLRenderer.hpp"
#include "Utilities.h"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


bool GLRenderer::init()
{
    try
    {
        loadShaders();
        loadTextures();
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
    for (auto i: Objects)
    {
        glDeleteVertexArrays(1, &(VAO));
        glDeleteBuffers(1, &(VBO));
        glDeleteBuffers(1, &(EBO));
    }
    
}

void GLRenderer::loadShaders()
{
    ourShader.reset(new Shader(vertexPath, fragPath));
}

void GLRenderer::loadObjects()
{
    Objects.push_back(Object({
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    }, 
    {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    }));

    for (auto i:Objects)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    for (auto i:Objects)
    {
        // bind the Vertex Array Object - VAO
        glBindVertexArray(VAO);

        // bind and set vertex buffer(s) - VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Note: "Add i.vertices.size()" * sizeof(GLfloat) when using vector.
        glBufferData(GL_ARRAY_BUFFER, i.vertices.size() * sizeof(GLfloat), (i.vertices.data()), GL_STATIC_DRAW);

        // Index Buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.indices.size() * sizeof(uint32_t), (i.indices.data()), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }


    // draw in wireframe polygons
    #if POLYGON_MODE
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif // POLYGON_MODE

}

void GLRenderer::loadTextures()
{
    // load and create a texture 
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void GLRenderer::motion()
{
    
}

void GLRenderer::draw() 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    ourShader->use();
    
    
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    // first container
    // ---------------
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // get their uniform location and set matrix (using glm::value_ptr)
    unsigned int transformLoc = glGetUniformLocation(ourShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // with the uniform matrix set, draw the first container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // second transformation
    // ---------------------
    transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scaleAmount = static_cast<float>(sin(glfwGetTime()));
    transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value

    // now with the uniform matrix being replaced with new transformations, draw it again.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}