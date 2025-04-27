#version 460

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture,texCoords);
}