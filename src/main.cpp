#include "window.h"
#include <stdio.h>

int main() {
    Window window;
    if (!window.init_window()) {
		printf("FAILED TO CREATE GLFW WINDOW!\n");
		return 1;
    }

    // main loop
	glfwMakeContextCurrent(window.window);

	while (!glfwWindowShouldClose(window.window)) {
	    glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLE_STRIP);
		{

			glVertex2d(-1.0, -1.0);
			glVertex2d(1.0, -1.0);
			glVertex2d(-1.0, 1.0);
			glVertex2d(1.0, 1.0);

		}
		glEnd();

	    glfwSwapBuffers(window.window);
	    glfwPollEvents();
	}

    window.cleanup();

    return 0;
}
