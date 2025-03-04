#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <map>


// Meant to abstract all glfw and opengl away and handling rendering.
// Any opengl or glfw calls should be handled through
// Also any thing to do with rendering should be handled through this too.
struct Window {
	GLFWwindow* window;
	GLuint vbo;
	GLuint vao;
	size_t frame;
	int width = 800;
	int height = 400;
	float last_time = 0.0;
	float delta_time = 0.0;

    bool mouse_locked = false;

    // boolean - success
    bool init_window();
    bool init_shaders();

    bool should_close();

	void clear(float r, float g, float b);
	void draw_quad();
	void gen_quad();

	void lock_mouse();
	void unlock_mouse();

	// Polls events, swaps the buffers and waits for the next frame.
	void end_frame();
    ~Window();
};


/*
// mee - wip fucky code for compile-time evaluation. kinda hacky though
// what do you think?

// MyUsername - Bro legitimately genius not sure why I didn't think to just use a template :skull:
// If your down to do it I would 100% be down, or if you want me to take a crack at it let me know

#define uniform_check(type) constexpr (std::is_same_v<T, type>)
template <typename T> void set_uniform(const char* key, T value) {
	GLint location = glGetUniformLocation(program_id, key);

	// evaluates at compile-time
	if uniform_check(int) {
		glUniform1i(location, value);

	} else if uniform_check(float) {
		glUniform1f(location, value);

	} else if uniform_check(glm::vec3) {
		glUniform3f(location, value.x, value.y, value.z);

	} else {
		//#error "ERROR: Unknown uniform type"
	}
}*/