cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
)

FetchContent_MakeAvailable(glfw)

project(opengl-raymarcher)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ./src)