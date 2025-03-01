#pragma once

#ifdef __unix__
#include <glad/glad.h>
#endif

class ShaderProgram {

public:
	// Creates a shader program that can be used when drawing geometry.
	ShaderProgram(const char *vertex_shader, const char *pixel_shader);

	const char *vertex_shader_source;
	const char *pixel_shader_source;

	GLuint program_id;
	GLuint shader_id;

	bool compiled = false;

	// Compiles the program and returns true is it succseeded
	bool compile();
	// Use the program for the next draw call.
	void use();
};