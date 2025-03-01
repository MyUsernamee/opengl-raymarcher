#pragma once

#include "config.h"
#include "debug.h"
#include "glad/glad.h"
#include "shader_compiler.h"

#include <GLFW/glfw3.h>

struct Window {
	GLFWwindow* window;
	size_t frame;
	ShaderCompiler shader_program;

	// boolean - success
	bool init_window();
	bool init_shaders();
	void cleanup();
};