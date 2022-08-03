#pragma once
#define GL_RENDERER_HPP 
#ifdef GL_RENDERER_HPP

#include "Object.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <vector>
#include <cstdlib>
#include <cstdint> 
#include <string> 
#include <array>
#include <memory>


class GLRenderer
{
    public:
        GLRenderer() = default;
        ~GLRenderer() = default;

        bool init();
        void loadObjects();
        void loadShaders();
        void loadTextures();
        void motion();
        void clean();

        void draw();

        // getters
        
        // setters
        void setShaderPaths(std::string vertexPath, std::string fragPath)
        {
            this -> vertexPath = vertexPath;
            this -> fragPath = fragPath;
        }

        struct shaderDeleter
        {
            void operator()(Shader *shader)
            {
                std::cout << "Destroying Shader" << std::endl;
                glDeleteProgram(shader->ID);
            }
        };
    float deg = 0.f, x = 0.f, y = 0.f, z = -3.0f, rotX = 1.0f, rotY = 1.0f, rotZ = 1.0f;
    private:
    std::string vertexPath;
    std::string fragPath;
    
    uint32_t VAO, VBO, EBO, texture;


    std::vector<Object> Objects;
    std::string errorInfo;
    std::unique_ptr<Shader, shaderDeleter> ourShader;
    std::vector<std::unique_ptr<Texture>> textures;

};
#endif // GL_RENDERER_HPP