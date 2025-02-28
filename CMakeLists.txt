cmake_minimum_required(VERSION 3.28)

find_package(OpenGL)

set(PROJECT_INCLUDES "./src glfw/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dont really understand what this does ngl
project(opengl-raymarcher)
file(GLOB_RECURSE SOURCES ./src/*.cpp ./src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

# setup C++ linking
target_link_libraries(${PROJECT_NAME} PRIVATE GL glfw)
target_include_directories(${PROJECT_NAME} PRIVATE "./include")
