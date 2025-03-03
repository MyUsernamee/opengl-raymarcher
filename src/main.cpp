#include "player.h"
#include "window.h"
#include <stdio.h>

// must be global so they can be accessed via callbacks
Player player;
Window window;

int main() {
	if (!window.init_window()) return 1;

	// define callbacks
	// TODO: is this dogshit?
	glfwSetCursorPosCallback(window.window, [](GLFWwindow* window, double x_pos, double y_pos){
		player.mouse_callback(window, x_pos, y_pos);
	});

	glfwSetKeyCallback(window.window, [](GLFWwindow* _, int key, int scancode, int action, int mods) {
		player.key_callback(key, scancode, action, mods);
	});

	// when window is resized
	glfwSetWindowSizeCallback(window.window, [](GLFWwindow* _, int width, int height) {
		window.width = width;
		window.height = height;
		glViewport(0, 0, width, height);
	});

	// main game loop (move if needed)
	while (!window.should_close()) {
		// player stuff
		player.process_movement();

		// TODO: maybe abstract into window (renderer) class?
		window.set_uniform("time", (float)glfwGetTime());
		window.set_uniform("eye_pos", player.pos);
		window.set_uniform("rotation", player.get_rotation_matrix());
		window.set_uniform("aspect_ratio", (float)window.width / (float)window.height);
		
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

	return 0;
}
