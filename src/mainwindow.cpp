#include "mainwindow.h"

#include "vs_test.h"
#include "ps_test.h"

// returns true on success, false otherwise
bool MainWindow::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 400, "Raymarcher", nullptr, nullptr);
	
#ifdef __unix
	if (!gladLoadGL(glfwGetProcAddress))
	{
		DEBUG_PRINT("UNIX: Failed to load glad");
	}
#endif

	if (!init_shaders())
	    DEBUG_PRINT("Failed to load shaders, see reason above.");

	//if (!compile_shaders())
	//    DEBUG_PRINT("Failed to compile shaders.");

	return window != nullptr;
}

bool MainWindow::init_shaders() {
	vertex_shaders.push_back(SHADER_VS_TEST);
	pixel_shaders.push_back(SHADER_PS_TEST);

	return true;
}

void MainWindow::cleanup() {
	glfwDestroyWindow(window);
	glfwTerminate();
}