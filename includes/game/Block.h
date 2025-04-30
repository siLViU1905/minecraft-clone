#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "../mine/VBO.h"
#include "../mine/VAO.h"
#include "../mine/EBO.h"
#include "../mine/Shader.h"
#include "../mine/TextureManager.h"
#include "../glm/glm.hpp"
#include "../mine/Camera.h"
#include "Light.h"

enum class BlockType
{
   GRASS,
   DIRT,
   STONE,
   AIR
};

class Block
{
    const Texture* texture;

    glm::mat4 model;

    glm::vec3 position;

    glm::vec3 scale;

    bool destroyed;

    float rotation;

    bool applyDownForce;

 public:
    BlockType type;   

    Block();

    void applyModel(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation = glm::vec3(1.f), float angle = 0.f);

    void applyCamera(const Camera& camera, Shader& shader);

    void applyLight(const Light& light, Shader& shader);

    glm::vec3& getPosition();

    const glm::vec3& getPosition() const;


    void render(const VAO& vao, Shader& shader);

    friend class WorldMap;
};

#endif // __BLOCK_H__