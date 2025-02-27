#include <stdio.h>
#include "mainwindow.h"

int main() {
	MainWindow window;
	if (!window.init()) {
		printf("FAILED TO CREATE GLFW WINDOW!\n");
		return 1;
	}

	// Main game loop
	glfwMakeContextCurrent(window.window);
	while (!glfwWindowShouldClose(window.window)) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glfwSwapBuffers(window.window);

		window.frame += 1;
	}

	window.cleanup();

	return 0;
}
