#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <cstring>
#include <string>
#include <vector>


class ShaderProgram {

public:
	// Creates a shader program that can be used when drawing geometry.
	ShaderProgram();

	GLuint program_id;
	std::vector<GLuint> shader_ids;
	std::vector<std::tuple<GLenum, std::string>> shader_sources;
	bool compiled = false;

	// Add a shader source and type to the shader program
	void add_shader(GLenum type, std::string source);

	// Compiles the program and returns true is it succseeded
	bool compile();
	// Use the program for the next draw call.
	void use();
};