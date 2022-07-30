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
        bool init();
        void loadObjects();
        void loadShaders();
        void clean();

        void draw();

        // getters
        
        // setters
        void setShaderPaths(std::string vertexPath, std::string fragPath)
        {
            this -> vertexPath = vertexPath;
            this -> fragPath = fragPath;
        }
    private:
    std::string vertexPath;
    std::string fragPath;

    
    uint32_t VAO, VBO;


    std::vector<Object> Objects;
    std::string errorInfo;

    std::shared_ptr<Shader> ourShader;

    
};
#endif // GL_RENDERER_HPP