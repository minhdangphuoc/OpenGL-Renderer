#define POLYGON_MODE 0

#include "GLRenderer.hpp"
#include "Utilities.h"

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
        glDeleteVertexArrays(1, &(VAO));
        glDeleteBuffers(1, &(VBO));
        // glDeleteBuffers(1, &(i.EBO));
    }
    
    glDeleteProgram(shaderProgram);
}

void GLRenderer::loadObjects()
{
    Objects.push_back(Object({
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    }));

    
    for (auto i: Objects)
    {
        for (auto j: i.vertices)
        {
            std::cout << j << " ";
        }
        std::cout << size(Objects) << "\n";
    }
    

    for (auto i:Objects)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

    }

    for (auto i:Objects)
    {
        // bind the Vertex Array Object - VAO
        glBindVertexArray(VAO);

        // bind and set vertex buffer(s) - VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Note: "Add i.vertices.size()" * sizeof(GLfloat) when using vector.
        glBufferData(GL_ARRAY_BUFFER, i.vertices.size() * sizeof(GLfloat), (i.vertices.data()), GL_STATIC_DRAW);
        
        // Configure vertex attributes(s) - EBO
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // Unbind array buffers
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glUseProgram(shaderProgram);


    // draw in wireframe polygons
    #if POLYGON_MODE
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif // POLYGON_MODE

}

void GLRenderer::loadVertexShaders()
{
    char * vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
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
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

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


        
    //     double  timeValue = glfwGetTime();
    //     float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    //     int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //     glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    for (auto i:Objects)
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}