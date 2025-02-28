#include "mainwindow.h"
#include "config.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "debug.h"
#include <vector>

//MainWindow::GLFWwindow* window = nullptr;
// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	shader_sources = new std::string[MAX_SHADER_COUNT];
	shader_c_str = new char *[MAX_SHADER_COUNT];
	shader_lengths = new unsigned int[MAX_SHADER_COUNT];

	if (!load_shaders())
	    DEBUG_PRINT("Failed to load shaders, see reason above.");

	if (!compile_shaders())
	    DEBUG_PRINT("Failed to compile shaders.");

	return window != nullptr;
}

bool MainWindow::load_shaders() {

	// Load a test shader from the disk
	std::ifstream input_stream;
	std::string file_data;

	input_stream.open("shaders/test.frag");

	if(!input_stream.is_open())
	{
	    DEBUG_PRINT("Failed to open shader file.");
	    return false;
	}

	input_stream >> file_data;

	shader_sources[0] = file_data;
	shader_c_str[0] = file_data.c_str();
	shader_lengths[0] = file_data.length();

	return true;
}

bool MainWindow::compile_shaders() { 
	
	// Hello

	auto shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_id, 1, shader_c_str, (const GLint*)shader_lengths);
	glCompileShader(shader_id);

	GLint isCompiled = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
	    DEBUG_PRINT("Error compiling shader.");

	    GLint max_length = 0;
	    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

	    std::vector<GLchar> errorLog(max_length);
	    glGetShaderInfoLog(shader_id, max_length, &max_length,
			       &errorLog[0]);

	    DEBUG_PRINT("%s", &errorLog[0]);
	    glDeleteShader(shader_id);
	    return false;
	}

	return true;
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