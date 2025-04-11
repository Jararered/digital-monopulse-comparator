cmake_minimum_required(VERSION 3.10)

project(digital-monopulse-comparator)

add_subdirectory(ext)

set(APP_SOURCES src/core/Application.cpp src/core/Simulation.cpp)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACES IMGUI_INTERFACE IMPLOT_INTERFACE GLAD_INTERFACE GLFW_INTERFACE LINALG_INTERFACE)
set(DEFINITIONS GLFW_INCLUDE_NONE)

set(OBJECTS_SOURCES 
    src/signal/SignalDisplayObject.cpp
    src/signal/SignalGenerator.cpp
)

add_executable(digital_monopulse_comparator src/main.cpp ${APP_SOURCES} ${OBJECTS_SOURCES})
target_include_directories(digital_monopulse_comparator PRIVATE ${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(digital_monopulse_comparator PRIVATE ${INTERFACES})
target_compile_definitions(digital_monopulse_comparator PRIVATE ${DEFINITIONS})
