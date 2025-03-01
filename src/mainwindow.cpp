#include "mainwindow.h"

#include <cstring>
#include "vs_test.h"
#include "ps_test.h"

// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	glfwMakeContextCurrent(window);

#ifdef __unix__
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_PRINT("UNIX: Failed to load glad");
		return false;
	}
#endif

	if (!init_shaders())
	    DEBUG_PRINT("Failed to load shaders, see reason above.");

	//if (!compile_shaders())
	//    DEBUG_PRINT("Failed to compile shaders.");

	return window != nullptr;
}

bool MainWindow::init_shaders() {
	vertex_shaders.push_back(SHADER_VS_TEST);
	pixel_shaders.push_back(SHADER_PS_TEST);

	// Calculate string length from null termination


	auto shader = glCreateShader(GL_FRAGMENT_SHADER);
	auto length = strlen(pixel_shaders.at(0)); 
	glShaderSource(shader, 1, (const GLchar* const*)pixel_shaders.data(), (GLint *)&length);

	glCompileShader(shader);

	int did_compile = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &did_compile);
	if (!did_compile) {
		// Get the length of error info.
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		// Get the actual info
		std::vector<GLchar> info(length);
		glGetShaderInfoLog(shader, length, &length, &info[0]);
		info.push_back(0); // Add null terminator

		DEBUG_PRINT("Error compiling shader: %s\n\nSource: %s", &info[0], pixel_shaders.at(0));
		return false;
	}

	
	auto program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);

	glUseProgram(program);

	return true;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}