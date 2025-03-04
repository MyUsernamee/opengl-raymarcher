#include "player.h"
#include "window.h"
#include <stdio.h>
#include <cmath> 

// must be global so they can be accessed via callbacks

#define CPP
static Player player;
static Window window;
#include "../shared/object.h"

using namespace glm;

vec3 max(vec3 a, float b) {
  return vec3(max(a.x, b), max(a.y, b), max(a.z, b));
}
vec3 min(vec3 a, float b) {
  return vec3(min(a.x, b), min(a.y, b), min(a.z, b));
}
vec3 mod(vec3 a, float b) {
	return vec3(
		std::fmod(a.x, b),
		std::fmod(a.y, b),
		std::fmod(a.z, b)
	);
}


#include "../shared/march.h"

int main() {
	if (!window.init_window()) return 1;

	add_object(create_object(SDF_MANDLEBROT));
	update_gpu_objects();

	// define callbacks
	// TODO: is this dogshit? 
	// Ngl yea. We should probably move this into the window somehow.
	glfwSetCursorPosCallback(window.window, [](GLFWwindow* window, double x_pos, double y_pos){
		player.mouse_callback(window, x_pos, y_pos);
	});

	glfwSetKeyCallback(window.window, [](GLFWwindow* _, int key, int scancode, int action, int mods) {

		if (key == GLFW_KEY_ESCAPE) {
			window.unlock_mouse();
		}

		player.key_callback(key, scancode, action, mods);
	});

	glfwSetMouseButtonCallback(
	    window.window,
	    [](GLFWwindow *_, int button, int action, int mods) {


			if (button == GLFW_MOUSE_BUTTON_1 && !window.mouse_locked) {
				window.lock_mouse();
			}

			if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
			  auto data =
			      march(player.pos, player.get_forward() * 1000.0f);
			  add_object(
			      create_object(SDF_SPHERE, INTERSECTION_UNION,
					    data.position, mat3(1.0), 0.01f));
			  update_gpu_objects();
			}

	});

	    // when window is resized
	    glfwSetWindowSizeCallback(window.window,
				      [](GLFWwindow *_, int width, int height) {
					window.width = width;
					window.height = height;
					glViewport(0, 0, width, height);
				      });

	// main game loop (move if needed)
	while (!window.should_close()) {
		// player stuff
		player.process_movement(sdf(player.pos));
		// TODO: maybe abstract into window (renderer) class?
		//window.set_uniform("time", (float)glfwGetTime());
		window.set_uniform("eye_pos", player.pos);
		window.set_uniform("rotation", player.get_rotation_matrix());
		window.set_uniform("aspect_ratio", (float)window.width / (float)window.height);
		
		window.clear(0.0, 0.0, 0.0);

		window.draw_quad();

		window.end_frame();


	}

	return 0;
}
