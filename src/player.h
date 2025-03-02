#include "window.h"	// for opengl includes

#define GLM_ENABLE_EXPERIMENTAL	// for quat
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

// stores information about the player
// including position and rotation
struct Player {
	vec2 prev_mouse;
	quat rotation = quat(1, 0, 0, 0);
	vec3 pos = vec3(-1.2, 0, 0);
	std::map<int, bool> key_down_map;	// TODO: bitmask w/ enum may be quicker lookup

	// x is forward, y is right, z is up
	mat3x3 get_rotation_matrix();
	vec3 get_forward();
	void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool is_key_down(int key);
};