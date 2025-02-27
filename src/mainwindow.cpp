#include "mainwindow.h"
#include <math.h>

//MainWindow::GLFWwindow* window = nullptr;
// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);

	return window != nullptr;
}

// returns true on success, false otherwise
bool MainWindow::main_loop() {
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glfwSwapBuffers(window);

		frame += 1;
	}

	return true;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}