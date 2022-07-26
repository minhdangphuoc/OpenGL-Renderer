#define POLYGON_MODE 0

#include "GLRenderer.hpp"
#include "Utilities.h"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <array>

bool GLRenderer::init()
{
    try
    {
        loadVertexShaders();
        loadFragShaders();
        linkShaders();
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
        glDeleteVertexArrays(1, &(i.VAO));
        glDeleteBuffers(1, &(i.VBO));
        // glDeleteBuffers(1, &(i.EBO));
    }
    
    glDeleteProgram(shaderProgram);
}

void GLRenderer::loadObjects()
{
    Objects.push_back(Object({
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    }));

    Objects.push_back(Object({
        // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    }));
    
    for (auto i: Objects)
    {
        for (auto j: i.vertices)
        {
            std::cout << j << " ";
        }
        std::cout << size(Objects) << "\n";

    }
    

    // Generate VAO, VBO, EBO
    // uint32_t * VAOs = &([=]() {
    //     std::vector<uint32_t> tmp;
    //     for(auto i:Objects){
    //         tmp.push_back(i.VAO);
    //     }
    //     return tmp;
    // }()[0]);
    // uint32_t * VBOs = &([=](){
    //     std::vector<uint32_t> tmp;
    //     for(auto i:Objects){
    //         tmp.push_back(i.VBO);
    //     }
    //     return tmp;
    // }()[0]);

    
    glGenVertexArrays(Objects.size(), VAOs);
    glGenBuffers(Objects.size(), VBOs);
    
    int tmp = 0;

    for (auto i:Objects)
    {
        // bind the Vertex Array Object - VAO
        glBindVertexArray(VAOs[tmp]);

        // bind and set vertex buffer(s) - VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[tmp]);
        glBufferData(GL_ARRAY_BUFFER, sizeof((i.vertices)), (i.vertices.data()), GL_STATIC_DRAW);
        
        // Configure vertex attributes(s) - EBO
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Set vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (tmp != 1) ? 3 * sizeof(float) : 0, (void*)0);
        glEnableVertexAttribArray(0);  
        glBindVertexArray(0);
        tmp++;
    }

    // Unbind array buffers
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 


    // draw in wireframe polygons
    #if POLYGON_MODE
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif // POLYGON_MODE

}

void GLRenderer::loadVertexShaders()
{
    char * vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    if (![=]() {
            int result;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
            return result;
        }())
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, &errorInfo[0]);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED " + errorInfo);
    }

}

void GLRenderer::loadFragShaders()
{
    char * fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (![=]() {
            int result;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
            return result;
        }())
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, &errorInfo[0]);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " + errorInfo);
    }
}

void GLRenderer::linkShaders()
{
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (![=]() {
            int result;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
            return result;
        }())
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, &errorInfo[0]);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED " + errorInfo);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void GLRenderer::draw() 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // then we draw the second triangle using the data from the second VAO
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}