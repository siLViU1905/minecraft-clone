#include "../GL/glad.h"
#include "../GLFW/glfw3.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

extern bool windowResized;
extern int WINDOW_WIDTH, WINDOW_HEIGHT;

void resizeFunc(GLFWwindow *window, int w, int h);

GLFWwindow *initGLFWGLAD();

ImGuiIO &initImGui(GLFWwindow *window);

void newFrame();

void renderFrame();

void terminateImGui();

void proccesEvents(GLFWwindow* window);

void terminateGLFW(GLFWwindow* window);

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );