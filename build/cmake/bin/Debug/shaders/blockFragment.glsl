#version 460 core

in vec2 texCoords;

uniform sampler2D myTexture;

out vec4 FragColor;

void main()
{
   FragColor = texture(myTexture, texCoords);
  //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}