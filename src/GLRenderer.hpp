#pragma once
#ifndef GL_RENDERER_HPP
#define GL_RENDERER_HPP 

#include "Shader.hpp"
#include "Texture.hpp"
#include "CameraSystem.hpp"
#include "Model.hpp"

#include <vector>
#include <cstdlib>
#include <cstdint> 
#include <array>
#include <map>
#include <unordered_map>
#include <memory>


class GLRenderer
{
    public:
        GLRenderer() = default;

        bool init();
        void loadObjects();
        void loadShaders();
        void setProjection();
        void clean();

        void draw();

        // getters
        
        // setters
        void setShaderPaths(std::string vertexPath, std::string fragPath)
        {
            this -> vertexPath = vertexPath;
            this -> fragPath = fragPath;
        }

        void setCamera(Camera * newCamera);

    
        float x = 1.f, y = 1.f, z = 1.f, rotX = 1.0f, rotY = 1.0f, rotZ = 1.0f, sX = 1.0f, sY = 1.0f, sZ = 1.0f;
        
        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
            
        // timing
        double deltaTime = 0.0f;
        double lastFrame = 0.0f;
        
        std::unique_ptr<Camera> camera;
        bool polyMode = false;
        float shininess = 2.f;
        std::array<float, 3> objectColor = {1.0f, 0.5f, 0.31f}, lightColor = {1.0f, 1.0f, 1.0f};

        int wHeight = 720, wWidth = 1080;
        std::unordered_map<std::string /* title */, std::unique_ptr<Model>> Objects;
    private:
        struct shaderDeleter
        {
            void operator()(Shader *shader)
            {
                std::cout << "Destroying Shader" << std::endl;
                glDeleteProgram(shader->ID);
                std::cout << "Destroyed Shader" << std::endl;
            }
        };
    std::string vertexPath;
    std::string fragPath;
    
    uint32_t VAO, VBO, EBO, texture;

    std::string errorInfo;

    // Renderer Components
    std::unordered_map<std::string /* title */, std::unique_ptr<Shader>> shaders;
};
#endif // GL_RENDERER_HPP