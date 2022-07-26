#pragma once
#define GL_RENDERER_HPP 
#ifdef GL_RENDERER_HPP

#include "Object.hpp"

#include <vector>
#include <cstdlib>
#include <cstdint> 
#include <string> 
#include <array>


class GLRenderer
{
    public:
        bool init();
        void loadObjects();
        void loadVertexShaders();
        void loadFragShaders();
        void linkShaders();
        void clean();

        void draw();

        // getters
        

    private:
    uint32_t vertexShader;
    uint32_t fragmentShader;
    uint32_t shaderProgram;
    unsigned int VBOs[2], VAOs[2];

    std::vector<Object> Objects;
    std::string errorInfo;

    
};
#endif // GL_RENDERER_HPP