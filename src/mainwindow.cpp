#include "mainwindow.h"
#include "config.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "debug.h"

//MainWindow::GLFWwindow* window = nullptr;
// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	shader_sources = new char *[MAX_SHADER_COUNT];
	shader_lengths = new unsigned int[MAX_SHADER_COUNT];

	return window != nullptr;
}

bool MainWindow::load_shaders() {

	// Load a test shader from the disk
	std::ifstream input_stream;
	std::string file_data;

	input_stream.open("shaders/test.frag");

	if(!input_stream.is_open())
	{
	    DEBUG_PRINT("Failed to open shader file.\n");
	    return false;
	}

	input_stream >> file_data;

	shader_sources[0] = new char[file_data]
}

// returns true on success, false otherwise
bool MainWindow::main_loop() {
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {

	    glClear(GL_COLOR_BUFFER_BIT);

	
		glBegin(GL_TRIANGLE_STRIP);
		{

			glVertex2d(-1.0, -1.0);
			glVertex2d(1.0, -1.0);
			glVertex2d(-1.0, 1.0);
			glVertex2d(1.0, 1.0);

		}
		glEnd();

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	    frame += 1;
	}

	return true;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}