#include "player.h"
#include <cmath>

#define GRAVITY -0.01f

mat3x3 Player::get_rotation_matrix() {
	return toMat3(rotation);
}

vec3 Player::get_forward() {
	return rotation * vec3(1, 0, 0);
}

vec3 Player::get_right() {
	return rotation * vec3(0, 1, 0);
}

vec3 Player::get_up() { return rotation * vec3(0, 0, 1); }

void Player::process_movement(float dist, float dt) {
	/*
	pos += velocity * dt;
	velocity += normalize(pos) * GRAVITY * dt;

	velocity -= dot(velocity, get_up()) * get_up();
	velocity -= dot(velocity, get_right()) * get_right();*/

	float speed = is_key_down(GLFW_KEY_LEFT_SHIFT) ? 0.3f : 0.1f;	// sprint
	speed *= clamp(dist, 0.00001f, 1.0f);

	if(is_key_down(GLFW_KEY_W)) {
		pos += get_forward() * speed;
	}

	if (is_key_down(GLFW_KEY_A)) {
		pos += get_right() * -speed;
	}

	if (is_key_down(GLFW_KEY_S)) {
		pos += get_forward() * -speed;
	}

	if (is_key_down(GLFW_KEY_D)) {
		pos += get_right() * speed;
	}
}

void Player::mouse_callback(GLFWwindow *window, double dx_pos, double dy_pos) {
	/*// Normalize to -1 : 1
	int width, height; glfwGetWindowSize(window, &width, &height);
	float x_pos = (float)dx_pos / (float)width * 2.0f - 1.0f;
	float y_pos = (float)dy_pos / (float)height * 2.0f - 1.0f;

	// delta calcs
    float sensitivity = 3.0f;
    float delta_x = (float)(x_pos - prev_mouse.x) * sensitivity;
    float delta_y = (float)(y_pos - prev_mouse.y) * sensitivity;

	prev_mouse = vec2(x_pos, y_pos);

    quat q_pitch = angleAxis(-delta_y, get_right());  // Pitch
    quat q_yaw   = angleAxis(-delta_x, get_forward());  // Yaw

    // yaw * rotation * pitch for camera-style rotation
    rotation = q_yaw * q_pitch * rotation;
    rotation = normalize(rotation);*/
	
	int width, height; glfwGetWindowSize(window, &width, &height);
	float x_pos = (float)dx_pos / (float)width * 2.0f - 1.0f;
	float y_pos = (float)dy_pos / (float)height * 2.0f - 1.0f;

	// delta calcs
    float sensitivity = 3.0f;
    float delta_x = (float)(x_pos - prev_mouse.x) * sensitivity;
    float delta_y = (float)(y_pos - prev_mouse.y) * sensitivity;

	prev_mouse = vec2(x_pos, y_pos);

    quat q_pitch = angleAxis(delta_y, vec3(0.0f, 1.0f, 0.0f));  // Pitch
    quat q_yaw   = angleAxis(delta_x, vec3(0.0f, 0.0f, 1.0f));  // Yaw

    // yaw * rotation * pitch for camera-style rotation
    rotation = q_yaw * rotation * q_pitch;
    rotation = normalize(rotation);
}

void Player::key_callback(int key, int scancode, int action, int mods) {
	key_down_map.insert_or_assign(key, action != GLFW_RELEASE);
}

bool Player::is_key_down(int key) {
	return key_down_map.count(key) ? key_down_map.at(key) : false;
}