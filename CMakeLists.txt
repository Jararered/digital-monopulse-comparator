cmake_minimum_required(VERSION 3.10)

project(digital-monopulse-comparator)

add_subdirectory(ext)

add_executable(digital_monopulse_comparator 
    src/main.cpp
    src/AntennaPlatform.cpp
    src/SignalProcessor.cpp
    src/TargetSimulator.cpp
    src/TrackerController.cpp
)

target_include_directories(digital_monopulse_comparator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(digital_monopulse_comparator
PRIVATE
    IMGUI_INTERFACE
    GLAD_INTERFACE
    GLFW_INTERFACE
    LINALG_INTERFACE
)

target_compile_definitions(digital_monopulse_comparator PRIVATE
    GLFW_INCLUDE_NONE
)