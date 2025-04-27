#include "includes/mine/init.h"
#include "includes/game/Block.h"
#include "includes/game/Crosshair.h"
#include <time.h>
#include <iostream>
#include "includes/glm/gtc/type_ptr.hpp"
#include "includes/game/WorldMap.h"
#include "includes/game/Hud.h"
#include "includes/game/Skybox.h"

std::string lookingAtBlock(BlockType bt)
{
    std::string rtr;
    switch (bt)
    {
    case BlockType::GRASS:
        rtr = "GRASS BLOCK";
        break;
    case BlockType::DIRT:
        rtr = "DIRT BLOCK";
        break;
    case BlockType::STONE:
        rtr = "STONE BLOCK";
        break;
    case BlockType::AIR:
        rtr = "AIR BLOCK";
        break;
    }

    return rtr;
}

int main()
{
    GLFWwindow *window = initGLFWGLAD();

    ImGuiIO &io = initImGui(window);

    Camera camera(window, glm::vec3(0.f, 1.f, 2.f), 2.f);

    Crosshair crosshair("shaders/crosshairVertex.glsl", "shaders/crosshairFragment.glsl");

    std::string OpenGLVendor = "Vendor: ";
    std::string OpenGLRenderer = "Renderer: ";
    std::string OpenGLVersion = "Version: ";
    std::string GLSLVersion = "GLSL Version: ";

    OpenGLVendor += OpenGL_Vendor;
    OpenGLRenderer += OpenGL_Renderer;
    OpenGLVersion += OpenGL_Version;
    GLSLVersion += GLSL_Version;

    srand(time(0));

    std::vector<std::string> faces{
        "textures/px.png",
        "textures/nx.png",
        "textures/py.png",
        "textures/ny.png",
        "textures/pz.png",
        "textures/nz.png"};

     
    WorldMap worldMap(&camera, glm::ivec3(-1, 0, -1), glm::ivec3(1, 1, 1));

    Skybox skybox(&camera, faces);


    Inventory inv(&worldMap);

    std::string worldInitTime = std::string("World init time: ") + std::to_string(worldMap.getInitTime() * 1000) + std::string("ms");

    double renderTime = 0.0;

    int buttonPressedNow = false, buttonPressedLast = false;

    while (!glfwWindowShouldClose(window))
    {
        if (windowResized)
        {
            camera.updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);
            windowResized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double startRenderTime = glfwGetTime();
        camera.update(window);

        worldMap.renderBlocks();

        skybox.render();

        crosshair.render();

        inv.render();

        buttonPressedNow = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

        buttonPressedLast = buttonPressedNow;

        newFrame();

        ImGui::Begin("Times");

        ImGui::Text(worldInitTime.c_str());

        ImGui::Text((std::string("Render time: ") + std::to_string(renderTime * 1000) + std::string("ms")).c_str());

        ImGui::End();

        ImGui::Begin("Position");

        ImGui::Text((std::string("x: ") + std::to_string(camera.getPosition().x) +
                     std::string("\ny: ") + std::to_string(camera.getPosition().y) +
                     std::string("\nz: ") + std::to_string(camera.getPosition().z))
                        .c_str());

        ImGui::End();

        ImGui::Begin("Looks at");

        ImGui::Text(lookingAtBlock(worldMap.lookingAt()).c_str());

        ImGui::End();

        ImGui::Begin("OpenGL Info");

        ImGui::Text(OpenGLVendor.c_str());

        ImGui::Text(OpenGLRenderer.c_str());

        ImGui::Text(OpenGLVersion.c_str());

        ImGui::Text(GLSLVersion.c_str());

        ImGui::End();

        renderFrame();

        glfwSwapBuffers(window);

        renderTime = glfwGetTime() - startRenderTime;

        proccesEvents(window);
        worldMap.proccesUserEvents(window);
    }

    terminateApp();
}