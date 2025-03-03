#include "player.h"
#include "window.h"
#include <stdio.h>

// must be global so they can be accessed via callbacks
Player player;
Window window;

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

#include "../shared/object.h"

static std::vector<Object> objects;
static size_t object_count;

#include <cmath>
#include "../shared/march.h"

int main() {
	if (!window.init_window()) return 1;

	objects = {
	    Object{SDF_SPHERE, INTERSECTION_UNION, mat4(1.0)}};
	object_count = objects.size();


	for (const auto& obj : objects) {
		printf("Object Type: %d\n", obj.shape);
		printf("Object Operation: %d\n", obj.intersection_type);
		printf("Object Matrix:\n");
		const float* matrix = (const float*)glm::value_ptr(obj.model_matrix);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				printf("%f ", matrix[i * 4 + j]);
			}
			printf("\n");
		}
		printf("\n");
	}

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
