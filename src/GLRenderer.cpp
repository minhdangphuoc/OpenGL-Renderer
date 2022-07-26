#define POLYGON_MODE 1

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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glDeleteProgram(shaderProgram);
}

void GLRenderer::loadObjects()
{

    const float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    // Generate VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind the Vertex Array Object - VAO
    glBindVertexArray(VAO);

    // bind and set vertex buffer(s) - VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Configure vertex attributes(s) - EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // Unbind array buffers
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}