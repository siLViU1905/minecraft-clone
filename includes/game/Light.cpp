#include "Light.h"
#include "../glm/gtc/matrix_transform.hpp"


void Light::move(const glm::vec3& offset)
{
    model = glm::mat4(1.f);
    position += offset;
    model = glm::translate(model,position);
}

void Light::scale(const glm::vec3& scale)
{
    model = glm::mat4(1.f);
    model = glm::scale(model, scale);
}

void Light::applyCamera(const Camera& camera, Shader& shader)
{
    shader.setMat4("camera.view", camera.getView());
    shader.setMat4("camera.projection", camera.getProjection());
}

void Light::render(const VAO& vao, Shader& shader)
{
    shader.setMat4("model", model);
    shader.setVec3("Color", color);

    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Light::Light()
{
    position = glm::vec3(0.f);
    ambient = glm::vec3(0.2f); 
    diffuse = glm::vec3(0.7f); 
    specular = glm::vec3(1.0f);
    color = glm::vec3(1.0f);   
    
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
    model = glm::mat4(1.f);
}
