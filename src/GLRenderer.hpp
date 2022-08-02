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



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
                // glDeleteProgram(shader->ID);
            }
        };
    float deg, x, y, z, rotX = 1.0f, rotY = 1.0f, rotZ = 1.0f;
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