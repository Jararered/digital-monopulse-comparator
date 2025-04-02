cmake_minimum_required(VERSION 3.10)

project(digital-monopulse-comparator)

add_subdirectory(ext)

set(APP_SOURCES src/Application.cpp src/Simulation.cpp src/Monopulse.cpp)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(INTERFACES IMGUI_INTERFACE GLAD_INTERFACE GLFW_INTERFACE LINALG_INTERFACE)
set(DEFINITIONS GLFW_INCLUDE_NONE)

add_executable(digital_monopulse_comparator src/main.cpp ${APP_SOURCES})
target_include_directories(digital_monopulse_comparator PRIVATE ${INCLUDE_DIRS})
target_link_libraries(digital_monopulse_comparator PRIVATE ${INTERFACES})
target_compile_definitions(digital_monopulse_comparator PRIVATE ${DEFINITIONS})

add_executable(monopulse_test src/test.cpp ${APP_SOURCES})
target_include_directories(monopulse_test PRIVATE ${INCLUDE_DIRS})
target_link_libraries(monopulse_test PRIVATE ${INTERFACES})
target_compile_definitions(monopulse_test PRIVATE ${DEFINITIONS})