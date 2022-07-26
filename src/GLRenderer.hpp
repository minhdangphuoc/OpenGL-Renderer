#pragma once
#define GL_RENDERER_HPP 
#ifdef GL_RENDERER_HPP

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

    std::string errorInfo;

    uint32_t VBO, VAO, EBO;
};
#endif // GL_RENDERER_HPP