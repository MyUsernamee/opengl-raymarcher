#include "player.h"

mat3x3 Player::get_rotation_matrix() {
	return toMat3(rotation);
}

vec3 Player::get_forward() {
	return rotation * vec3(1, 0, 0);
}

void Player::mouse_callback(GLFWwindow *window, double dx_pos, double dy_pos) {
	// Normalize to -1 : 1
	int width, height; glfwGetWindowSize(window, &width, &height);
	float x_pos = (float)dx_pos / (float)width * 2.0f - 1.0f;
	float y_pos = (float)dy_pos / (float)height * 2.0f - 1.0f;

	// delta calcs
    float sensitivity = 3.0f;
    float delta_x = (float)(x_pos - prev_mouse.x) * sensitivity;
    float delta_y = (float)(prev_mouse.y - y_pos) * sensitivity;

	prev_mouse = vec2(x_pos, y_pos);

    quat q_pitch = angleAxis(delta_y, vec3(0.0f, 1.0f, 0.0f));  // Pitch
    quat q_yaw   = angleAxis(delta_x, vec3(0.0f, 0.0f, 1.0f));  // Yaw

    // yaw * rotation * pitch for camera-style rotation
    rotation = q_yaw * rotation * q_pitch;
    rotation = normalize(rotation);
}

void Player::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	key_down_map.insert_or_assign(key, action != GLFW_RELEASE);
}

bool Player::is_key_down(int key) {
	return key_down_map.count(key) ? key_down_map.at(key) : false;
}