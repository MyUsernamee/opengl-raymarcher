#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class ShaderProgram {

private:

	GLuint shader_id;
	GLuint program_id;
	GLuint buffer_id;
	std::string vertex_shader;
	std::string fragment_shader;

	bool built = false;
	bool has_buffer = false;

	// Creates a shader and compiles it with a given type
	static GLuint compile_shader(GLenum shader_type, std::string source);
	static bool check_compile_error(GLuint shader_id); // Checks if there was a compile error then prints and returns false if there was.
	static bool check_program_error(GLenum type, GLuint program_id); // Same as above but for programs.

public:	

	ShaderProgram(std::string vertex_shader, std::string fragment_shader);
	ShaderProgram(); // Not used, empty.

	bool compile();
	void add_uniform_buffer();
	void use();

	bool is_compiled();

	~ShaderProgram();

    void set_uniform(const char *key, float value);
    void set_uniform(const char *key, int value);
    void set_uniform(const char *key, glm::vec3 value);
    void set_uniform(const char *key, glm::mat3x3 value);

    void set_uniform_buffer(const char *key, void *buffer, size_t size);


};