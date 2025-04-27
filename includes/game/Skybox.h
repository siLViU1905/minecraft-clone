#ifndef __SKYBOX_H__
#define __SKYBOX_H__
#include "Block.h"

class Skybox
{
  VBO skyVbo;
  VAO skyVao;
  unsigned int cubemapTex;
  Shader skyShader;
  Texture skyTexture;

  const Camera* camera;

  public:
  Skybox(const Camera* camera, const std::vector<std::string>& faces);

  void render();
};

#endif // __SKYBOX_H__