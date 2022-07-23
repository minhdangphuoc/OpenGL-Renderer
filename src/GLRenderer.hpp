#pragma once
#define GL_RENDERER_HPP 
#ifdef GL_RENDERER_HPP
class GLRenderer
{
    public:
        void init();
        void loadObjects();
        void loadShaders();
        void clean();

    private:
};
#endif // GL_RENDERER_HPP