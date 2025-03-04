#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>	// glm::value_ptr
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <map>

struct Shader {
	GLenum shader_type;
	const char* source;

	Shader(GLenum shader_type, const char* source) {
		this->shader_type = shader_type;
		this->source = source;
	}
};

// Meant to abstract all glfw and opengl away and handling rendering.
// Any opengl or glfw calls should be handled through
// Also any thing to do with rendering should be handled through this too.
struct Window {
	GLFWwindow* window;
	GLuint vbo;
	GLuint vao;
	GLuint program_id;
	GLuint buffer_id;
	size_t frame;
	int width = 800;
	int height = 400;
	float last_time = 0.0;
	float delta_time = 0.0;
	std::vector<GLuint> shader_ids;
    std::vector<Shader> shader_sources;

    bool mouse_locked = true;

    // boolean - success
    bool init_window();
    bool init_shaders();
    void add_shader(GLenum shader_type, const char *source);
    bool compile_shaders();
    void cleanup_shaders();
    //void cleanup();

    bool should_close();

	void clear(float r, float g, float b);
	void draw_quad();

	void lock_mouse();
	void unlock_mouse();


	// Polls events, swaps the buffers and waits for the next frame.
	void end_frame();

    void set_uniform(const char *key, float value);
    void set_uniform(const char *key, int value);
    void set_uniform(const char *key, glm::vec3 value);
    void set_uniform(const char *key, glm::mat3x3 value);

    void set_uniform_buffer(const char *key, void *buffer, size_t size);

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