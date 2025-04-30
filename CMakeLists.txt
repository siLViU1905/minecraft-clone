cmake_minimum_required(VERSION 3.26.0)
project(sMine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/cmake/bin/Debug)
set(CMAKE_SOLUTION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/cmake)

include_directories(${CMAKE_SOURCE_DIR}/includes)

find_package(OpenGL REQUIRED)


link_directories(${CMAKE_SOURCE_DIR}/lib)

set(SOURCE_FILES
        includes/GL/glad.cpp
        includes/imgui/imgui.cpp
        includes/imgui/imgui_draw.cpp
        includes/imgui/imgui_widgets.cpp
        includes/imgui/imgui_tables.cpp
        includes/imgui/imgui_impl_glfw.cpp
        includes/imgui/imgui_impl_opengl3.cpp
        main.cpp
        includes/mine/Shader.cpp
        includes/mine/VBO.cpp
        includes/mine/VAO.cpp
        includes/mine/EBO.cpp
        includes/mine/Camera.cpp
        includes/mine/Texture.cpp
        includes/mine/init.cpp
        includes/game/Block.cpp
        includes/mine/TextureManager.cpp
        includes/game/Crosshair.cpp
        includes/game/WorldMap.cpp
        includes/game/Hud.cpp
        includes/game/Skybox.cpp
        includes/game/Light.cpp
)

add_executable(sMine ${SOURCE_FILES})
set_target_properties(sMine PROPERTIES OUTPUT_NAME "main")


if(WIN32)
    target_link_libraries(sMine
            ${OPENGL_LIBRARIES}
            glfw3
    )
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_VIDEO_LIBRARY CoreVideo)
    target_link_libraries(sMine
            ${OPENGL_LIBRARIES}
            glfw
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${CORE_VIDEO_LIBRARY}
    )
else()
    target_link_libraries(sMine
            ${OPENGL_LIBRARIES}
            glfw
            dl
    )
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")