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

	glfwSetKeyCallback(window.window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		player.key_callback(window, key, scancode, action, mods);
	});

	// main game loop (move if needed)
	while (!window.should_close()) {

		// player movement
		float speed = player.is_key_down(GLFW_KEY_LEFT_SHIFT) ? 0.003f : 0.001f;

		if(player.is_key_down(GLFW_KEY_W)) {
			player.pos += player.get_forward() * speed;
		}

		if (player.is_key_down(GLFW_KEY_A)) {
			player.pos += player.get_right() * -speed;
		}

		if (player.is_key_down(GLFW_KEY_S)) {
			player.pos += player.get_forward() * -speed;
		}

		if (player.is_key_down(GLFW_KEY_D)) {
			player.pos += player.get_right() * speed;
		}



		window.set_uniform("time", (float)glfwGetTime());
		window.set_uniform("aspect_ratio", (float)window.width() / (float)window.height());
		window.set_uniform("eye_pos", player.pos);
		window.set_uniform("rotation", player.get_rotation_matrix());
		
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
