cmake_minimum_required(VERSION 3.10)

# IMGUI INTERFACE

add_library(IMGUI_INTERFACE INTERFACE)

target_include_directories(IMGUI_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_sources(IMGUI_INTERFACE INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_opengl3.cpp
)

# GLAD

add_library(GLAD_INTERFACE INTERFACE)

target_include_directories(GLAD_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/inc
)

target_sources(GLAD_INTERFACE INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)

# GLFW

add_library(GLFW_INTERFACE INTERFACE)

target_include_directories(GLFW_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
)