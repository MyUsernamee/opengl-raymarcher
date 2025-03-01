#pragma once

#include "config.h"
#include "debug.h"
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <iostream>

#include "shader_compiler.h"

struct MainWindow {
	GLFWwindow* window;
	size_t frame;

	// boolean - success
	bool init_window();
	bool init_shaders();
	void cleanup();
};