#version 460 core

layout(location = 0) in vec3 aPos;

struct Camera
{
    mat4 view;
    mat4 projection;
};

uniform Camera camera;

uniform mat4 model;

void main()
{
    gl_Position = camera.projection * camera.view * model * vec4(aPos, 1.0);
}