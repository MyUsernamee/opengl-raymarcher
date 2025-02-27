#include <GLFW/glfw3.h>

struct MainWindow {
	GLFWwindow* window;
	size_t frame;

	// boolean - success
	bool init();
	void cleanup();
};