#pragma once

#include "config.h"
#include "debug.h"
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

struct MainWindow {
	GLFWwindow* window;
	size_t frame;

	std::vector<const char*> pixel_shaders;
	std::vector<const char*> vertex_shaders;

	// boolean - success
	bool init_window();
	bool init_shaders();
	void cleanup();
};