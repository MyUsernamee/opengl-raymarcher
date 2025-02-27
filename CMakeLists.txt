cmake_minimum_required(VERSION 3.31.6)

find_package(OpenGL REQUIRED)

add_subdirectory(glfw)

set(PROJECT_INCLUDES glfw/include) 
set(PROJECT_LINKS glfw;OpenGL::GL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# dont really understand what this does ngl
project(opengl-raymarcher)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# setup C++ linking
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINKS})