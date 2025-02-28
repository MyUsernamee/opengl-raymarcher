#include "../include/glad/glad.h" // Generated file that loads OpenGL and imports all extensions
#include <GLFW/glfw3.h>

int main() {

  glfwInit();
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World!", nullptr, nullptr);

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    return 1; 
  glfwSwapInterval(1);

  while (!glfwWindowShouldClose(window)){

    glViewport(0, 0, 1280, 1280);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

	return 0;
}
