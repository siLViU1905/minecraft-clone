#ifndef __HUD_H__
#define __HUD_H__
#include "WorldMap.h"

class Inventory
{
   Shader slotShader;
   Shader itemShader;
   VBO slotVbo;
   VAO slotVao;
   VAO itemVao;

   static constexpr float square[16]={
        -0.1f,-0.1f,  0.f, 0.f,
        0.f, -0.1f,   1.f, 0.f,
          0.f, 0.f,   1.f, 1.f,
        -0.1f, 0.f,   0.f, 1.f
   };

   glm::mat4 model;

   glm::vec3 position;
   glm::vec3 scale;

   WorldMap* worldMap;

   public:
   Inventory(WorldMap* worldMap);

   void render();

};


class Hud
{
  
};

#endif // __HUD_H__