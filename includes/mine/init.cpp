#include "init.h"
#include <stdio.h>
#include <string>
#include <iostream>

bool windowResized = false;

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

void resizeFunc(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
    windowResized = true;
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
}

GLFWwindow *initGLFWGLAD()
{
    if (!glfwInit())
        return nullptr;

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);
       

    GLFWwindow *window = glfwCreateWindow(1080, 720, "Minecraft", nullptr, nullptr);
    WINDOW_WIDTH = 1080;
    WINDOW_HEIGHT = 720;

    if (!window)
        return nullptr;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return nullptr;

    glfwSetFramebufferSizeCallback(window, resizeFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
   // glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return window;
}

ImGuiIO &initImGui(GLFWwindow *window)
{
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();
    return io;
}

void newFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void renderFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void terminateImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool leftAltPressed = false, enterPressed = false, leftCtrlPressed = false;

void proccesEvents(GLFWwindow *window)
{
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwGetKey(window, GLFW_KEY_B))
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    leftAltPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
    enterPressed = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;

    if (leftAltPressed && enterPressed)
        glfwMaximizeWindow(window);

    leftCtrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

    if (leftCtrlPressed && enterPressed)
        glfwSetWindowSize(window, 1080, 720);
}

void terminateGLFW(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *message,
                                const void *userParam)
{
    std::string severityStr;
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        severityStr = "GL_ERROR";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        severityStr = "GL_WARNING";
    else if (severity == GL_DEBUG_SEVERITY_LOW)
        severityStr = "GL_INFO";
    else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        severityStr = "GL_NOTIFICATION";

    std::cout << severityStr << " [type=0x" << std::hex << type
              << ", severity=0x" << severity << "]: " << message << std::endl;
}
