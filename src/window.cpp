#include "window.h"

#include "vs_test.h" // SHADER_VS_TEST
#include "ps_test.h" // SHADER_PS_TEST

// returns true on success, false otherwise
bool Window::init_window() {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, 0);	// TODO: fix me

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DEBUG_PRINT("ERROR: Failed to load glad");
		return false;
	}

	if (!init_shaders())
	    DEBUG_PRINT("ERROR: Failed to load shaders");

	return window != nullptr;
}

bool Window::init_shaders() {
	add_shader(GL_VERTEX_SHADER, SHADER_VS_TEST);
	add_shader(GL_FRAGMENT_SHADER, SHADER_PS_TEST);

	if (!compile_shaders()) return false;
	
	glUseProgram(program_id);
	return true;
}

void Window::add_shader(GLenum shader_type, const char* source) {
	shader_sources.push_back(Shader(shader_type, source));
}

void Window::cleanup_shaders() {
	// must be invalid program_id
	if (shader_ids.empty()) return;

	// delete/detach shaders
	// DOCUMENTATION:
		// deleting a shader in opengl won't actually delete the shader until all
		// references (to respective programs) are deleted
		// this means we can delete a shader as soon as its created and when the program 
		// is removed, the shader will automatically kill itself
		// for this example though, I have avoided this for a more "memory safe" approach
		// and, to potentially avoid overhead
	for (GLuint shader_id : shader_ids) {
		glDetachShader(program_id, shader_id);
		glDeleteShader(shader_id);
	}

	// kill program
	glDeleteProgram(program_id);
	shader_ids.clear();
}

bool Window::compile_shaders() {
	bool full_compile = true;	// did all shaders sucessfully compile?

	cleanup_shaders();
	for (Shader shader_source: shader_sources) {

		// create shader
		GLuint shader_id = glCreateShader(shader_source.shader_type);
		glShaderSource(shader_id, 1, &shader_source.source, NULL);
		glCompileShader(shader_id);

		// is shit fucked?
		GLint did_compile; glGetShaderiv(shader_id, GL_COMPILE_STATUS, &did_compile);
		if (did_compile) {
			shader_ids.push_back(shader_id);

		} else { // yep, shits fucked
			int length; glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

			// print error
			std::vector<GLchar> info(length);
			glGetShaderInfoLog(shader_id, length, &length, info.data());
			DEBUG_PRINT("ERROR: Failed to compile shader: %s", info.data());
			glDeleteShader(shader_id); 	// dont forget to free our shader

			full_compile = false;
		}
	}

	// don't link shaders
	if (!full_compile) return false;

	// create "program" (?)
	program_id = glCreateProgram();

	// link our shaders
	for (GLuint shader_id : shader_ids) {
		glAttachShader(program_id, shader_id);
	}

	glLinkProgram(program_id);
	shader_sources.clear();

	return true;
}

// TODO: Probably want to store these or something and have a hook that updates them but for now this works.
int Window::width() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

int Window::height() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

bool Window::should_close() { return glfwWindowShouldClose(window); }

// sometimes the best solutions are the simplest
// Honestly fair, just felt like it was a bit tedious to write all the possible different data types but :shrug:
#define quick_set_uniform(main_type, gl_func, ...)           \
void Window::set_uniform(const char* key, main_type self) {  \
    GLint location = glGetUniformLocation(program_id, key);  \
    gl_func(location, __VA_ARGS__);                          \
}

quick_set_uniform(float, glUniform1f, self);
quick_set_uniform(int, glUniform1i, self);
quick_set_uniform(glm::vec3, glUniform3f, self.x, self.y, self.z);
quick_set_uniform(glm::mat3x3, glUniformMatrix3fv, 1, false, glm::value_ptr(self));

void Window::cleanup() {
	cleanup_shaders();

	glfwDestroyWindow(window);
	glfwTerminate();
}