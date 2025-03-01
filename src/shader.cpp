#include "shader.h"

#include "debug.h"
#include <cstring>
#include <vector>

ShaderProgram::ShaderProgram(const char* vertex_shader, const char* pixel_shader) {
	vertex_shader_source = vertex_shader;
	pixel_shader_source = pixel_shader;
	program_id = 0;
	shader_id = 0;
}

bool ShaderProgram::compile() {

	auto shader = glCreateShader(GL_FRAGMENT_SHADER);
	auto px_length = strlen(pixel_shader_source);
	auto vt_length = strlen(vertex_shader_source);

	const char *sources[2] = {pixel_shader_source, vertex_shader_source};
	const int lengths[2] = {px_length, vt_length};

	// Tell open gl to compile these source files
	glShaderSource(shader, 2, sources, lengths);
	
	// Actually compile them
	glCompileShader(shader);

	int did_compile = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &did_compile); // Get if the shader compiled
	if (!did_compile) {
		// Get the length of error info.
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		// Get the actual info
		std::vector<GLchar> info(length);
		glGetShaderInfoLog(shader, length, &length, &info[0]);
		info.push_back(0); // Add null terminator

		DEBUG_PRINT("Error compiling shader: %s", &info[0]);
		return false;
	}

	
	// Create the actual program and save it
	auto program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);

	program_id = program;
	shader_id = shader;

	compiled = true;

	return true;
}

void ShaderProgram::use() {
	
	if (!compiled) {
		DEBUG_PRINT("Attemping to use uncompiled shader program.");
		return;
	}

	glUseProgram(program_id); 
}
