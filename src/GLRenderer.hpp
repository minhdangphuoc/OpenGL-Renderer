#pragma once
#define GL_RENDERER_HPP 
#ifdef GL_RENDERER_HPP

#include "Object.hpp"
#include "Shader.hpp"

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
                // glDeleteProgram(shader->ID);
            }
        };
    private:
    std::string vertexPath;
    std::string fragPath;
    
    uint32_t VAO, VBO, EBO;


    std::vector<Object> Objects;
    std::string errorInfo;

    std::unique_ptr<Shader, shaderDeleter> ourShader;
    uint32_t texture;
};
#endif // GL_RENDERER_HPP