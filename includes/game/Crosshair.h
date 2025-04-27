#ifndef __CROSSHAIR_H__
#define __CROSSHAIR_H__

#include "../mine/VBO.h"
#include "../mine/VAO.h"
#include "../mine/Shader.h"
#include "../glm/glm.hpp"
#include "../mine/Camera.h"

class Crosshair
{
    VBO vbo;
    VAO vao;
    Shader shader;

    static constexpr float lines[8] = {
        0.0f, -0.02f,  
        0.0f, 0.02f,   
        -0.02f, 0.0f,  
        0.02f, 0.0f };


public:
    Crosshair(const char *vertexShaderPath, const char *fragmentShaderPath);

    void render();
};

#endif // __CROSSHAIR_H__