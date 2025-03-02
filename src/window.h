#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>	// glm::value_ptr
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Shader {
	GLenum shader_type;
	const char* source;

	Shader(GLenum shader_type, const char* source) {
		this->shader_type = shader_type;
		this->source = source;
	}
};

struct Window {
	GLFWwindow* window;
	GLuint program_id;
	size_t frame;
	std::vector<GLuint> shader_ids;
    std::vector<Shader> shader_sources;

	// boolean - success
	bool init_window();
	bool init_shaders();
	void add_shader(GLenum shader_type, const char* source); // Adds a shader to the render pipeline with the given type
	bool compile_shaders();
	void cleanup_shaders();
	void cleanup();

	int width(); // Gets the current height in pixels
	int height(); // Get the current width in pixels

	bool should_close();
	bool is_key_down(int key);

	void set_uniform(const char* key, float value);
	void set_uniform(const char* key, int value);
	void set_uniform(const char* key, glm::vec3 value);
	void set_uniform(const char* key, glm::mat3x3 value);
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