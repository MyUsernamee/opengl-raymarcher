cmake_minimum_required(VERSION 3.31.6)

set(PROJECT_LINKS "glfw")
set(PROJECT_INCLUDES "./src glfw/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get required submodules
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)

# dont really understand what this does ngl
project(opengl-raymarcher)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# setup C++ linking
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINKS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})