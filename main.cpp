#include "includes/mine/auxiliary.h"
#include "includes/game/Block.h"
#include "includes/game/Crosshair.h"
#include <time.h>
#include <iostream>
#include "includes/glm/gtc/type_ptr.hpp"
#include "includes/game/WorldMap.h"
#include "includes/game/Hud.h"
#include "includes/game/Skybox.h"
#include <fstream>


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

#ifdef _WIN32
   setCaptionColor(RGB(0,0,120));
#endif

   float startFrameTime = 0.f, frameTime = 0.f;

    glfwSwapInterval(0);

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

    float ts = (float)glfwGetTime();

    std::string renderTimeStr;
    std::string frameTimeStr ;

    srand(time(0));

    std::vector<std::string> faces{
        "textures/px.png",
        "textures/nx.png",
        "textures/py.png",
        "textures/ny.png",
        "textures/pz.png",
        "textures/nz.png"};

     
       glm::ivec3 mapStart = glm::ivec3(-1, 0, -1);
         glm::ivec3 mapEnd = glm::ivec3(1, 1, 1);

    WorldMap* worldMap = new WorldMap(&camera, mapStart, mapEnd);
    
    worldMap->light.position = glm::vec3(0.f, 1.f, 0.f);
    worldMap->light.scale(glm::vec3(1.f));

    Skybox skybox(&camera, faces);


    Inventory inv(worldMap);

    std::string worldInitTime = std::string("World init time: ") + std::to_string(worldMap->getInitTime() * 1000) + std::string("ms");

    float renderTime = 0.f;

    int buttonPressedNow = false, buttonPressedLast = false;

    while (!glfwWindowShouldClose(window))
    {
        startFrameTime = (float)glfwGetTime();
        if (windowResized)
        {
            camera.updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);
            windowResized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       float startRenderTime = (float)glfwGetTime();
        camera.update(window);

        worldMap->renderBlocks();

        worldMap->renderLights();

        skybox.render();

        crosshair.render();

        inv.render();

        buttonPressedNow = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

        buttonPressedLast = buttonPressedNow;

        newFrame();

        ImGui::Begin("Info");

        ImGui::Text(worldInitTime.c_str());

        if((float)glfwGetTime() - ts > 0.5f)
        {
            ts = (float)glfwGetTime();
            renderTimeStr = "Render time: ";
            frameTimeStr = "FPS: ";
            renderTimeStr += std::to_string(renderTime * 1000) + std::string("ms");
            frameTimeStr += std::to_string(1.f / frameTime);
        }

        ImGui::Text(renderTimeStr.c_str());

        ImGui::Text(frameTimeStr.c_str());

        ImGui::Text((std::string("RAM usage: ") + std::to_string(getRAMUsage() / 0x100000) + std::string("MB")).c_str());

        ImGui::End();

        ImGui::Begin("Position");

        ImGui::Text((std::string("x: ") + std::to_string(camera.getPosition().x) +
                     std::string("\ny: ") + std::to_string(camera.getPosition().y) +
                     std::string("\nz: ") + std::to_string(camera.getPosition().z))
                        .c_str());

        ImGui::End();

        ImGui::Begin("Looks at");

        ImGui::Text(lookingAtBlock(worldMap->lookingAt()).c_str());

        ImGui::End();

        ImGui::Begin("OpenGL Info");

        ImGui::Text(OpenGLVendor.c_str());

        ImGui::Text(OpenGLRenderer.c_str());

        ImGui::Text(OpenGLVersion.c_str());

        ImGui::Text(GLSLVersion.c_str());

        ImGui::End();


        ImGui::Begin("Light");

        ImGui::SliderFloat3("Light position", glm::value_ptr(worldMap->light.position), -20.f, 20.f);
        ImGui::SliderFloat3("Light color", glm::value_ptr(worldMap->light.color), 0.f, 1.f);
        ImGui::SliderFloat3("Light ambient", glm::value_ptr(worldMap->light.ambient), 0.f, 1.f);
        ImGui::SliderFloat3("Light diffuse", glm::value_ptr(worldMap->light.diffuse), 0.f, 1.f);
        ImGui::SliderFloat3("Light specular", glm::value_ptr(worldMap->light.specular), 0.f, 1.f);
        ImGui::SliderFloat("Light constant", &worldMap->light.constant, 0.f, 1.f);
        ImGui::SliderFloat("Light linear", &worldMap->light.linear, 0.f, 1.f);
        ImGui::SliderFloat("Light quadratic", &worldMap->light.quadratic, 0.f, 1.f);

        if(ImGui::Button("Save"))
        {
            std::ofstream lightFile("options/lightValues.txt", std::ios::out | std::ios::trunc);

            if(lightFile.is_open())
            {
                lightFile << worldMap->light.position.x << " " << worldMap->light.position.y << " " << worldMap->light.position.z << "\n";
                lightFile << worldMap->light.color.x << " " << worldMap->light.color.y << " " << worldMap->light.color.z << "\n";
                lightFile << worldMap->light.ambient.x << " " << worldMap->light.ambient.y << " " << worldMap->light.ambient.z << "\n";
                lightFile << worldMap->light.diffuse.x << " " << worldMap->light.diffuse.y << " " << worldMap->light.diffuse.z << "\n";
                lightFile << worldMap->light.specular.x << " " << worldMap->light.specular.y << " " << worldMap->light.specular.z << "\n";
                lightFile << worldMap->light.constant << " " << worldMap->light.linear << " " << worldMap->light.quadratic;
                lightFile.close();
            }
        }

        if(ImGui::Button("Load"))
        {
            std::ifstream lightFile("options/lightValues.txt");

            if(lightFile.is_open())
            {
                lightFile >> worldMap->light.position.x >> worldMap->light.position.y >> worldMap->light.position.z;
                lightFile >> worldMap->light.color.x >> worldMap->light.color.y >> worldMap->light.color.z;
                lightFile >> worldMap->light.ambient.x >> worldMap->light.ambient.y >> worldMap->light.ambient.z;
                lightFile >> worldMap->light.diffuse.x >> worldMap->light.diffuse.y >> worldMap->light.diffuse.z;
                lightFile >> worldMap->light.specular.x >> worldMap->light.specular.y >> worldMap->light.specular.z;
                lightFile >> worldMap->light.constant >> worldMap->light.linear >> worldMap->light.quadratic;
                lightFile.close();
            }
        }

        ImGui::End();

        ImGui::Begin("World");

        if(ImGui::SliderInt3("Map start", glm::value_ptr(mapStart), -20, 0))
            worldMap->resizeWorld(mapStart, mapEnd);
        
        if(ImGui::SliderInt3("Map end", glm::value_ptr(mapEnd), 0, 20))
        worldMap->resizeWorld(mapStart, mapEnd);

        if(ImGui::Button("Save"))
        {
            std::ofstream worldFile("options/worldValues.txt", std::ios::out | std::ios::trunc);

            if(worldFile.is_open())
            {
                worldFile << mapStart.x << " " << mapStart.y << " " << mapStart.z << "\n";
                worldFile << mapEnd.x << " " << mapEnd.y << " " << mapEnd.z;
                worldFile.close();
            }
        }

        if(ImGui::Button("Load"))
        {
            std::ifstream worldFile("options/worldValues.txt");

            if(worldFile.is_open())
            {
                worldFile >> mapStart.x >> mapStart.y >> mapStart.z;
                worldFile >> mapEnd.x >> mapEnd.y >> mapEnd.z;
                worldFile.close();
            }
        }
        

        ImGui::End();

        renderFrame();

        glfwSwapBuffers(window);

        renderTime = (float)glfwGetTime() - startRenderTime;

        proccesEvents(window);
        worldMap->proccesUserEvents(window);

        frameTime = (float)glfwGetTime() - startFrameTime;
    }
    delete worldMap;
    terminateApp();
}