cmake_minimum_required(VERSION 3.10)

# IMGUI INTERFACE
add_library(IMGUI_INTERFACE INTERFACE)

target_include_directories(IMGUI_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_sources(IMGUI_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

# GLAD INTERFACE
add_library(GLAD_INTERFACE INTERFACE)

target_include_directories(GLAD_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/inc
)

target_sources(GLAD_INTERFACE INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c
)

# GLFW INTERFACE
add_library(GLFW_INTERFACE INTERFACE)

target_include_directories(GLFW_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/inc
)

target_link_libraries(GLFW_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib/glfw3.lib
)

# add_subdirectory(eigen)

add_library(LINALG_INTERFACE INTERFACE)

target_include_directories(LINALG_INTERFACE INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/linalg/inc
)