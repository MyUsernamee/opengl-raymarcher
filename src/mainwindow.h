#include "glad/glad.h"
#include <GLFW/glfw3.h>

struct MainWindow {
	GLFWwindow* window;
	size_t frame;

	std::string* shader_sources;
	const char **shader_c_str;
	unsigned int *shader_lengths;

	// boolean - success
	bool init_window();
	bool load_shaders();
	bool compile_shaders();
	bool main_loop();
	void cleanup();
};