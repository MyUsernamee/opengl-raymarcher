cmake_minimum_required(VERSION 3.28)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/shaders_out)

# actual includes
set(PROJECT_LIBRARIES glfw)
set(PROJECT_INCLUDES include;glfw/include;${SHADER_OUT_DIR})

# generate shader files
file(MAKE_DIRECTORY ${SHADER_OUT_DIR})
file(GLOB SHADER_FILES "${SHADER_DIR}/*.glsl")
foreach(shader_file ${SHADER_FILES})
    get_filename_component(shader_name ${shader_file} NAME_WE)
	string(TOUPPER ${shader_name} shader_name_upper)	# toupper

	file(READ ${shader_file} shader_code)
    set(header_content "#define SHADER_${shader_name_upper} R\"(\n${shader_code}\n)\"")
    file(WRITE ${SHADER_OUT_DIR}/${shader_name}.glsl "${header_content}")
endforeach()

# dont really understand what this does ngl
project(opengl-raymarcher)
file(GLOB_RECURSE SOURCES ./src/*.cpp ./src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})

# setup linking
if (UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE GL;${PROJECT_LIBRARIES})
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES};${PROJECT_LIBRARIES})
endif()