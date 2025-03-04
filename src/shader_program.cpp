#include "shader_program.h"

#include <vector>
#include "debug.h"
#include <glm/gtc/type_ptr.hpp>	// glm::value_ptr

ShaderProgram::ShaderProgram() {

}

ShaderProgram::ShaderProgram(std::string _vertex_shader, std::string _fragment_shader) {

	vertex_shader = _vertex_shader;
	fragment_shader = _fragment_shader;

	compile();

}

GLuint ShaderProgram::compile_shader(GLenum shader_type, std::string source) {

	auto shader_id = glCreateShader(shader_type);
	auto c_str = source.c_str();
	auto size = (GLint)source.size();
	glShaderSource(shader_id, 1, &c_str, &size);
	glCompileShader(shader_id);

	auto compiled = ShaderProgram::check_compile_error(shader_id);	

	if (!compiled) {
		DEBUG_PRINT("FAILED TO COMPILE SHADER.");
		return -1;
	}

	return shader_id;

}

bool ShaderProgram::check_compile_error(GLuint id) {
	GLint passed;
	glGetShaderiv(id, GL_COMPILE_STATUS, &passed);
	if(!passed) {
		int length; glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// print error
		std::vector<GLchar> info(length);
		glGetShaderInfoLog(id, length, &length, info.data());
		DEBUG_PRINT("ERROR: Failed to compile shader: %s", info.data());
		glDeleteShader(id);
		return false;
	}
	return true;
}

bool ShaderProgram::check_program_error(GLenum type, GLuint id) {
	GLint passed;
	glGetProgramiv(id, type, &passed);
	if(!passed) {
		int length; 
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

		// print error
		std::vector<GLchar> info(length);
		glGetProgramInfoLog(id, length, &length, info.data());
		DEBUG_PRINT("ERROR: Failed to compile shader: %s", info.data());
		glDeleteProgram(id);
		return false;
	}
	return true;
}


bool ShaderProgram::compile() {

	auto fragment_shader_id = ShaderProgram::compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
	auto vertex_shader_id = ShaderProgram::compile_shader(GL_VERTEX_SHADER, vertex_shader);

	if (fragment_shader_id == -1 || vertex_shader_id == -1) {
		DEBUG_PRINT("FAILED TO COMPILE SHADER PROGRAM.");
		return false;
	}

	// create "program" (?)
	program_id = glCreateProgram(); // Yea its like the rendering pipeline. The program is the entire pipeline from vertex shader to ps shader
	glAttachShader(program_id, fragment_shader_id);
	glAttachShader(program_id, vertex_shader_id);
	glLinkProgram(program_id);

	auto linking_passed = ShaderProgram::check_program_error(GL_LINK_STATUS, program_id);
	if (!linking_passed) {
		DEBUG_PRINT("FAILED TO LINK SHADER");
	}

	glValidateProgram(program_id);

	auto validation_passed = ShaderProgram::check_program_error(GL_VALIDATE_STATUS, program_id);
	if (!validation_passed) {
		DEBUG_PRINT("FAILED TO LINK SHADER");
	}

	built = true;

	return true;
}

void ShaderProgram::use() {
	if (!built) {
		DEBUG_PRINT("Shader program used before compilation");
		return;
	}

	glUseProgram(program_id);
}

ShaderProgram::~ShaderProgram () {
	DEBUG_PRINT("TODO:::")	;
}

// sometimes the best solutions are the simplest
// Honestly fair, just felt like it was a bit tedious to write all the possible different data types but :shrug:
#define quick_set_uniform(main_type, gl_func, ...)                             \
  void ShaderProgram::set_uniform(const char *key, main_type self) {                  \
    GLint location = glGetUniformLocation(program_id, key);                    \
    if (location == -1)                                                         \
      DEBUG_PRINT("FAILED TO FIND UNIFORM"); \
	gl_func(location, __VA_ARGS__); \
}

quick_set_uniform(float, glUniform1f, self);
quick_set_uniform(int, glUniform1i, self);
quick_set_uniform(glm::vec3, glUniform3f, self.x, self.y, self.z);
quick_set_uniform(glm::mat3x3, glUniformMatrix3fv, 1, false, glm::value_ptr(self));

void ShaderProgram::add_uniform_buffer() {
	if (has_buffer)
		return;
	glGenBuffers(1, &buffer_id);
	has_buffer = true;
}

void ShaderProgram::set_uniform_buffer(const char* key, void* buffer, size_t size) {

	if(!has_buffer)
		return;

    // Get the index of the uniform block
    GLuint blockIndex = glGetUniformBlockIndex(program_id, key);
    
    // Choose a binding point (here we use 0; change if needed)
    GLuint bindingPoint = 0;
    glUniformBlockBinding(program_id, blockIndex, bindingPoint);
    
    // Bind our uniform buffer and send the data
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_DYNAMIC_DRAW);
    
    // Bind the whole buffer to the uniform block binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer_id);
    
    // Unbind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}