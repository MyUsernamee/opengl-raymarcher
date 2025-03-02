cmake_minimum_required(VERSION 3.28)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/shaders_out)

set(SHARED_DIR ${CMAKE_SOURCE_DIR}/shared)

# actual includes
set(PROJECT_LIBRARIES glfw)
set(PROJECT_INCLUDES include;glm;glfw/include;${SHADER_OUT_DIR})

file (GLOB_RECURSE SHADER_INCLUDES "${SHARED_DIR}/*.h")
set(shader_include_source "")
foreach(shader_include ${SHADER_INCLUDES})
	file(READ "${shader_include}" file_content)
	string(APPEND shader_include_source "${file_content}")
	string(STRIP "${shader_include_source}" shader_include_source)
endforeach()

# generate shader files
file(MAKE_DIRECTORY ${SHADER_OUT_DIR})
file(GLOB SHADER_FILES "${SHADER_DIR}/*.glsl")
foreach(shader_file ${SHADER_FILES})
    get_filename_component(shader_name ${shader_file} NAME_WE)
	string(TOUPPER ${shader_name} shader_name_upper)	# toupper
	file(READ ${shader_file} shader_code)

	set(VERSION_DIRECTIVE "")
	string(REGEX MATCH "#version ([^\n]*)" version "${shader_code}")	
	string(REGEX REPLACE "#version ([^\n]*)" "" shader_code "${shader_code}")


	set(final_file "")
	string(APPEND final_file "${version}\n")
	string(APPEND final_file "${shader_include_source}\n")
	string(APPEND final_file "${shader_code}\n")
	string(STRIP "${final_file}" final_file)
	
    set(header_content "#define SHADER_${shader_name_upper} R\"(${final_file})\"")
    file(WRITE ${SHADER_OUT_DIR}/${shader_name}.h "${header_content}")
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