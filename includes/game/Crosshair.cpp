#include "Crosshair.h"
#include "../glm/gtc/matrix_transform.hpp"
Crosshair::Crosshair(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    vbo.setData(lines, sizeof(lines));

    vao.linkAttrib(vbo, 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    vao.enableAttrib(0);

    shader.autoCompileAndLink(vertexShaderPath,fragmentShaderPath);
}

void Crosshair::render()
{
    glDisable(GL_DEPTH_TEST);

    shader.use();

    vao.bind();

    glLineWidth(1.f); 
    glDrawArrays(GL_LINES, 0, 4);

   glEnable(GL_DEPTH_TEST);
}
