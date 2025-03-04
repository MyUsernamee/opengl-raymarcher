#include "window.h"

#include "vs_test.h" // SHADER_VS_TEST
#include "ps_test.h" // SHADER_PS_TEST
#include "ps_scale.h"

const glm::vec2 verticies[4] {
	glm::vec2(-1.0f, -1.0f),
	glm::vec2(1.0f, -1.0f),
	glm::vec2(-1.0f, 1.0f),
	glm::vec2(1.0f, 1.0f)
};

bool Window::init_window() {
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, 0);	// TODO: fix me
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(width, height, "Raymarcher", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		DEBUG_PRINT("ERROR: Failed to load glad");
		return false;
	}
	glEnable(GL_DEBUG_OUTPUT);

	shader_scale = ShaderProgram(SHADER_VS_TEST, SHADER_PS_SCALE);

	glGenFramebuffers(1, &small_buffer);

	glGenTextures(1, &small_texture);
	glBindTexture(GL_TEXTURE_2D, small_texture );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width / 4, height / 4, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, small_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, small_texture, 0);	

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		DEBUG_PRINT("BUFFER NOT DONE.");
	}

	// Create buffer for the uniform buffer for objects.



	return window != nullptr;
}

void Window::gen_quad() {
	
	// Creates a vertex buffer object to store our verticies
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, &verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Creates a vertex attribute array (needed to draw the vertices).
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


}

bool Window::should_close() { return glfwWindowShouldClose(window); }

void Window::clear(float r, float g, float b) {
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::draw_quad() {

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Window::start_frame() {

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, small_buffer);	
	glViewport(0, 0, width / 4, height / 4);

}

void Window::end_frame() {

	shader_scale.use();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, small_texture);

	draw_quad();

	glfwSwapBuffers(window);
	glfwPollEvents();

	delta_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

}


Window::~Window() {

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::lock_mouse() {
	mouse_locked = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::unlock_mouse() {
	mouse_locked = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}