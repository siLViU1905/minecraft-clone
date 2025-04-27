#include "Hud.h"
#include "../glm/ext/matrix_transform.hpp"
#include <iostream>
Inventory::Inventory(WorldMap *worldMap) : worldMap(worldMap)
{
    slotVbo.setData(square, sizeof(square));

    slotVao.linkAttrib(slotVbo, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    slotVao.enableAttrib(0);

    itemVao.linkAttrib(slotVbo, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    itemVao.enableAttrib(0);

    itemVao.linkAttrib(slotVbo, 2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    itemVao.enableAttrib(1);

    slotShader.autoCompileAndLink("shaders/slotVertex.glsl", "shaders/slotFragment.glsl");

    itemShader.autoCompileAndLink("shaders/itemVertex.glsl", "shaders/itemFragment.glsl");

    position = {-0.33f, -0.805f, 0.f};

    scale = {0.69f, 1.f, 1.f};

    model = {1.f};
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
}

void Inventory::render()
{
    slotShader.use();
    slotShader.setMat4("model", model);

    slotVao.bind();

    glLineWidth(3.f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    auto itemTex = worldMap->pickedBlock();

    if (itemTex.first)
    {
        textureManager.bindTexture(*(itemTex.first));
        itemShader.use();
        itemShader.setInt("myTexture", itemTex.second);

        itemVao.bind();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
