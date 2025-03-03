#include "player.h"
#include "window.h"
#include <stdio.h>
#define CPP
#include "../shared/object.h"
#include <cmath>

static std::vector<Object> objects;
static size_t object_count;

// must be global so they can be accessed via callbacks
static Player player;
static Window window;

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

	objects = {
	    Object{SDF_MANDLEBROT, INTERSECTION_UNION, mat4(1.0)},
		Object{SDF_SPHERE, INTERSECTION_SUBTRACT, mat4(0.8)}};
	object_count = objects.size();

	window.set_uniform_buffer("ObjectBlock", (void *)objects.data(),
				  objects.size() * sizeof(Object));
	window.set_uniform("object_count", (int)objects.size());

	// define callbacks
	// TODO: is this dogshit? 
	// Ngl yea. We should probably move this into the window somehow.
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

	glClearColor(1.0, 0.0, 0.0, 1.0);
	// main game loop (move if needed)
	while (!window.should_close()) {
		// player stuff
		player.process_movement(sdf(player.pos));

		// TODO: maybe abstract into window (renderer) class?
		//window.set_uniform("time", (float)glfwGetTime());
		window.set_uniform("eye_pos", player.pos);
		window.set_uniform("rotation", player.get_rotation_matrix());
		window.set_uniform("aspect_ratio", (float)window.width / (float)window.height);
		
	    glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glfwSwapBuffers(window.window);
		glfwPollEvents();
	
	}

	return 0;
}
