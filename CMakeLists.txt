cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Local include folder (GLAD + GLM)
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(SOURCES
    src/main.cpp
    src/glad.c
)

add_executable(OpenGL_Project ${SOURCES})

target_link_libraries(OpenGL_Project
    glfw
    OpenGL::GL
)