#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <cstring>
#include <string>
#include <vector>

struct Shader {
	GLenum shader_type;
	const char* source;

	Shader(GLenum shader_type, const char* source) {
		this->shader_type = shader_type;
		this->source = source;
	}
};

struct ShaderCompiler {
	// Creates a shader program that can be used when drawing geometry.
	ShaderCompiler();

	GLuint program_id;
	std::vector<GLuint> shader_ids;
	std::vector<Shader> shader_sources;
	bool compiled = false;

	// Add a shader source and type to the shader program
	void add_shader(GLenum type, const char* source);

	// Compiles the program and returns true is it succseeded
	bool compile();

	// Use the program for the next draw call.
	void use();
};