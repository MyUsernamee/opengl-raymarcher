#include "mainwindow.h"
#include <math.h>

//MainWindow::GLFWwindow* window = nullptr;
// returns true on success, false otherwise
bool MainWindow::init() {
	if (!glfwInit()) return false;
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	return window != nullptr;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}