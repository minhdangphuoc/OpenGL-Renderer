#pragma once
#ifndef SHAPE_HPP
#define SHAPE_HPP


#include <vector>
#include <cstdint> 
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <iostream>
#include <map>

#include "Shader.hpp"
#include "Object.hpp"
#include "Material.hpp"


class Shape: public Object
{
public:
    Shape() = default;
    ~Shape() = default;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    void draw(Shader * shader) {
        std::cout<<std::endl;
    }
    void clean() {
        std::cout<<std::endl;
    }

    // Shape & operator= (Shape const & shape)
    // {
    //     return *this;
    // }

    void setMaterial(const Material & newMaterial) override
    {
        this -> material = newMaterial;
    }

    Material material = Material(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.5f,0.5f,0.5f),0.5f);
    unsigned int selected = 0;
    
private:
    bool idMaterialInit = false;
};


class Box : public Shape
{
public:
    ~Box() = default;
    Box(){
        vertices = {
        // positions        // Normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,       
        0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
        };

        indices = {  
            0, 1, 2, 2, 3, 0, // front
            0, 4, 5, 5, 1, 0, // right
            4, 7, 6, 6, 5, 4, // back
            7, 3, 2, 2, 6, 7, // left
            0, 3, 7, 7, 4, 0, // bottom
            1, 5, 6, 6, 2, 1 // top
        };

        loadObjBuffer();
    };
    void loadObjBuffer()
    {
        if (!isLoad)
        {
            #if (DEBUG)
                std::cout<< "loadObjBuffer() in Cube" <<std::endl;
            #endif
            
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            // bind the Vertex Array Object - VAO
            glBindVertexArray(VAO);

            // bind and set vertex buffer(s) - VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Note: "Add i.vertices.size()" * sizeof(GLfloat) when using vector.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), (vertices.data()), GL_STATIC_DRAW);

            // Index Buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), (indices.data()), GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
    }

    void draw(Shader * shader)
    {
        shader->setVec3("material.ambient", glm::vec3(
            material.Ambient.x, 
            material.Ambient.y, 
            material.Ambient.z
        ));
            
        shader->setVec3("material.diffuse", glm::vec3(
                material.Diffuse.x, 
                material.Diffuse.y, 
                material.Diffuse.z
        ));

        shader->setVec3("material.specular", glm::vec3(
                material.Specular.x, 
                material.Specular.y, 
                material.Specular.z
        ));
        
        shader->setFloat("material.shininess", material.Shininess);
        shader->setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    void clean() override
    {
        glDeleteVertexArrays(1, &(VAO));
        glDeleteBuffers(1, &(VBO));
        glDeleteBuffers(1, &(EBO));
    }
    private:
    static inline bool isLoad = false;
    static inline uint32_t VBO, VAO, EBO;
};


#endif