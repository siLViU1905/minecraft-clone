#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

struct Camera
{
   mat4 view;
   mat4 projection;
};

uniform mat4 model;

uniform Camera camera;

out vec3 FragPos;

out vec3 Normal;

void main()
{
    gl_Position = camera.projection * camera.view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos,1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = aTexCoords;
}