#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "../mine/Shader.h"
#include "../mine/Camera.h"
#include "../mine/VAO.h"

struct Light
{
 glm::vec3 position;
 glm::vec3 diffuse;
 glm::vec3 ambient;
 glm::vec3 specular;
 glm::vec3 color;

 float constant;
 float linear;
 float quadratic;

 void move(const glm::vec3& offset);

 void scale(const glm::vec3& scale);

 void applyCamera(const Camera& camera, Shader& shader);

 void render(const VAO& vao, Shader& shader);

  Light();

 private:
 glm::mat4 model;
};

#endif // __LIGHT_H__