#include "mainwindow.h"

#include <cstring>
#include "vs_test.h"
#include "ps_test.h"
#include "shader.h"

// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_PRINT("UNIX: Failed to load glad");
		return false;
	}

	if (!init_shaders())
	    DEBUG_PRINT("Failed to load shaders, see reason above.");

	//if (!compile_shaders())
	//    DEBUG_PRINT("Failed to compile shaders.");

	return window != nullptr;
}

bool MainWindow::init_shaders() {
  ShaderProgram *shader = new ShaderProgram();
  shader->add_shader(GL_VERTEX_SHADER, SHADER_VS_TEST);
  shader->add_shader(GL_FRAGMENT_SHADER, SHADER_PS_TEST);
  if (!shader->compile())
    return false;
  shader->use();
  return true;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}