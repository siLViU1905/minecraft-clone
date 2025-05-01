#include "Block.h"
#include "../glm/gtc/matrix_transform.hpp"

void Block::applyModel(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation, float angle)
{
    this->position = position;
    this->scale = scale;
    model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(angle), rotation);
}

void Block::applyCamera(const Camera &camera, Shader &shader)
{
    shader.setMat4("camera.projection", camera.getProjection());
    shader.setMat4("camera.view", camera.getView());
    shader.setVec3("viewPos", camera.getPosition());
}

void Block::applyLight(const Light& light, Shader& shader)
{
    shader.setVec3("lights[0].position", light.position);
    shader.setVec3("lights[0].color", light.color);
    shader.setVec3("lights[0].diffuse", light.diffuse);
    shader.setVec3("lights[0].ambient", light.ambient);
    shader.setVec3("lights[0].specular", light.specular);
    shader.setFloat("lights[0].constant", light.constant);
    shader.setFloat("lights[0].linear", light.linear);
    shader.setFloat("lights[0].quadratic", light.quadratic);
}

glm::vec3 &Block::getPosition()
{
    return position;
}

const glm::vec3 &Block::getPosition() const
{
    return position;
}

void Block::render(const VAO &vao, Shader &shader)
{
    textureManager.bindTexture(*texture);
       
    shader.setMat4("model", model);

    shader.setInt("material.diffuse", (int)type);

    shader.setVec3("material.specular", glm::vec3(1.f, 1.f, 1.f));

    shader.setFloat("material.shininess", 32.f);

    shader.setInt("lightsNumber", 1);

    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Block::Block() : destroyed(false), rotation(0.f),applyDownForce(false)
{
    rotationTime = (float)glfwGetTime();
}
