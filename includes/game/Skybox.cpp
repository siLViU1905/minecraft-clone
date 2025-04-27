#include "Skybox.h"
#include "../mine/stb_image.h"

Skybox::Skybox(const Camera* camera, const std::vector<std::string>& faces):camera(camera)
{
    float skyboxVertices[] = {          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    skyVbo.setData(skyboxVertices,sizeof(skyboxVertices));

    skyVao.linkAttrib(skyVbo, 0, 3 , GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    skyVao.enableAttrib(0);

    skyShader.autoCompileAndLink("shaders/skyVertex.glsl", "shaders/skyFragment.glsl");

    skyTexture = textureManager.generateTexture();

    skyTexture.loadCubemap(faces);
}

void Skybox::render()
{
    glm::mat4 skyView = glm::mat4(glm::mat3(camera->getView()));

    glDepthFunc(GL_LEQUAL);
    skyShader.use();
    skyShader.setMat4("view", skyView);
    skyShader.setMat4("projection", camera->getProjection());

    textureManager.bindCubemap(skyTexture);

    skyVao.bind();

    glDrawArrays(GL_TRIANGLES,0,36);
    glDepthFunc(GL_LESS);
}
