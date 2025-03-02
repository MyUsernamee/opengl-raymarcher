#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

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
	void add_shader(GLenum shader_type, const char* source);
	bool compile_shaders();
	void cleanup_shaders();
	void cleanup();

	void set_uniform(const char* key, float value);
	void set_uniform(const char* key, int value);
	void set_uniform(const char* key, glm::vec3 value);
};


/*
// mee - wip fucky code for compile-time evaluation. kinda hacky though
// what do you think?

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