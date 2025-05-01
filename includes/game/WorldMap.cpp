#include "WorldMap.h"
#include <thread>
#include <iostream>
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

WorldMap::WorldMap(const Camera *camera) : camera(camera), mapStart(glm::ivec3(-1, 0, -1)), mapEnd(glm::ivec3(1, 0, 1))
{
    initTime = glfwGetTime();
    mapSize = mapEnd - mapStart;
    initBuffers();
    initTextures();
    initShaders();
    generateBlocks();
    initTime -= glfwGetTime();
    initTime = -initTime;
    lightRotationAngle = 0.f;
    lightRotationTime = (float)glfwGetTime();
}

WorldMap::WorldMap(const Camera *camera, const glm::ivec3 &mapStart, const glm::ivec3 &mapEnd) : camera(camera),
                                                                                                 mapStart(mapStart), mapEnd(mapEnd)
{
    initTime = glfwGetTime();
    mapSize = mapEnd - mapStart;
    initBuffers();
    initTextures();
    initShaders();
    generateBlocks();
    initTime -= glfwGetTime();
    initTime = -initTime;
    lightRotationAngle = 0.f;
    lightRotationTime = (float)glfwGetTime();
}

void WorldMap::initBuffers()
{
    blockVbo.setData(cube, sizeof(cube));

    blockVao.linkAttrib(blockVbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    blockVao.enableAttrib(0);

    blockVao.linkAttrib(blockVbo, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    blockVao.enableAttrib(1);

    blockVao.linkAttrib(blockVbo, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    blockVao.enableAttrib(2);

    lightVbo.setData(cube, sizeof(cube));

    lightVao.linkAttrib(lightVbo, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    lightVao.enableAttrib(0);
}

void WorldMap::initTextures()
{
    for (int i = 0; i < 3; ++i)
        textures[i] = textureManager.generateTexture();

    textures[0].loadTexture("textures/grassTex.png");
    textures[1].loadTexture("textures/dirtTex.png");
    textures[2].loadTexture("textures/stoneTex.png");
}

void WorldMap::initShaders()
{
    blockShader.autoCompileAndLink("shaders/blockVertex.glsl", "shaders/blockFragment.glsl");
    lightShader.autoCompileAndLink("shaders/lightVertex.glsl", "shaders/lightFragment.glsl");
}

void WorldMap::generateBlocks()
{
    for (int z = mapStart.z; z < mapEnd.z; z++)
        for (int y = mapStart.y; y < mapEnd.y; y++)
            for (int x = mapStart.x; x < mapEnd.x; x++)
            {
                auto pos = glm::vec3(x, y, z);

                int texNum = rand() % 3;

                blocks.emplace(std::make_pair(pos, Block()));
                blocks[pos].position = pos;
                blocks[pos].applyModel(pos, glm::vec3(1.f));
                blocks[pos].type = (BlockType)texNum;
                blocks[pos].texture = &textures[texNum];
            }
}

void WorldMap::updateBlocks()
{
    for (auto &pair : blocks)
        if (pair.second.destroyed)
        {
            pair.second.rotation += (float)glfwGetTime() - pair.second.rotationTime;

            pair.second.rotationTime = (float)glfwGetTime();

            if(pair.second.rotation > 360.f)
                pair.second.rotation = 0.f;

            pair.second.applyModel(pair.second.position, glm::vec3(0.3f), glm::vec3(0.f, 1.f, 0.f), pair.second.rotation++);
            /* if (pair.second.applyDownForce)
             {
                 auto blockPosUnder = pair.second.position - glm::vec3(0.f, 1.f, 0.f);
                 if (blocks.find(blockPosUnder) == blocks.end())
                     pair.second.applyModel(pair.second.position - glm::vec3(0.f, 0.2f, 0.f), pair.second.scale);
                 else
                     pair.second.applyDownForce = false;
             }*/
        }
}

void WorldMap::updateLights()
{
    light.model = glm::mat4(1.f);

    /*float deltaTime = (float)glfwGetTime() - lightRotationTime;
    lightRotationAngle += deltaTime * 10.f;
    lightRotationTime = (float)glfwGetTime();
    light.model = glm::rotate(light.model, glm::radians(lightRotationAngle), glm::vec3(0.f, 0.f, 1.f));*/

    light.model = glm::translate(light.model, light.position);

    //light.position = glm::vec3(light.model[0]);
    
    light.model = glm::scale(light.model, glm::vec3(1.f));
    
    light.applyCamera(*camera, lightShader);
   
}

void WorldMap::renderBlockMultiThreaded(WorldMap *map, int start, int end)
{
    auto it = map->blocks.begin();

    if (start)
        std::advance(it, start);

    for (int i = start; i < end; ++i)
    {
        it->second.applyCamera(*(map->camera), map->blockShader);
        it->second.render(map->blockVao, map->blockShader);
        ++it;
    }
}

void WorldMap::UserEvents::pollEvents(GLFWwindow *window)
{
    mouseLeftClickThisFrame = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

    mouseLeftClick = (mouseLeftClickThisFrame && !mouseClickLastFrame) ? true : false;

    mouseClickLastFrame = mouseLeftClickThisFrame;
}

WorldMap::UserEvents::UserEvents()
{
    mouseLeftClick = mouseLeftClickThisFrame = mouseClickLastFrame = false;
}

void WorldMap::proccesUserEvents(GLFWwindow *window)
{
    userEvents.pollEvents(window);

    if (userEvents.mouseLeftClick)
        destroyBlock();
}

void WorldMap::destroyBlock()
{
    glm::vec3 pos = camera->getPosition();
    glm::vec3 dir = camera->getFront();
    dir = glm::normalize(dir);

    float maxDistance = 5.0f;
    float step = 0.2f;

    for (float distance = 0; distance <= maxDistance; distance += step)
    {
        glm::vec3 checkPos = pos + dir * distance;

        glm::vec3 blockPos = glm::vec3(
            std::round(checkPos.x),
            std::round(checkPos.y),
            std::round(checkPos.z));

        if (blocks.find(blockPos) != blocks.end())
        {
            // blocks.erase(blockPos);
            blocks[blockPos].destroyed = true;
            blocks[blockPos].applyDownForce = true;
            return;
        }
    }
}

std::pair<const Texture*,int> WorldMap::pickedBlock()
{
    for (auto &pair : blocks)
        if (pair.second.destroyed)
        {
            auto diff = glm::abs(pair.second.position - camera->getPosition());
            if (diff.x < 0.5f && diff.y < 0.5f && diff.z < 0.5f)
            {
                return std::make_pair(pair.second.texture,(int)pair.second.type);
            }
        }
    return std::make_pair(nullptr, (int)BlockType::AIR);
}

void WorldMap::renderBlocks()
{
    updateBlocks();
    for (auto &block : blocks)
    {
        block.second.applyCamera(*camera, blockShader);
        block.second.applyLight(light, blockShader);
        block.second.render(blockVao, blockShader);
    }

}

void WorldMap::renderLights()
{
    updateLights();
    light.render(lightVao, lightShader);
}

const double &WorldMap::getInitTime() const
{
    return initTime;
}

BlockType WorldMap::lookingAt() const
{
    glm::vec3 pos = camera->getPosition();
    glm::vec3 dir = camera->getFront();
    dir = glm::normalize(dir);

    float maxDistance = 5.0f;
    float step = 0.2f;

    for (float distance = 0; distance <= maxDistance; distance += step)
    {
        glm::vec3 checkPos = pos + dir * distance;

        glm::vec3 blockPos = glm::vec3(
            std::round(checkPos.x),
            std::round(checkPos.y),
            std::round(checkPos.z));

        if (blocks.find(blockPos) != blocks.end())
            return blocks.at(blockPos).type;
    }
    return BlockType::AIR;
}

void WorldMap::resizeWorld(const glm::ivec3 &mapStart, const glm::ivec3& mapEnd)
{
    this->mapStart = mapStart;
    this->mapEnd = mapEnd;
    mapSize = mapEnd - mapStart;
    blocks.clear();
    generateBlocks();
}
