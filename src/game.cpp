
#include "game.h"

// TODO: remove this class, it doesn't need to exist
// a simple Camera struct that holds player location / rotation data is good enough
// theres too much abstraction going on here

Game *Game::instance = nullptr; // This is a null ptr to start.

Game::Game() {
	if (!Game::get_instance()) {
		Game::instance = this;
	}
}

Game *Game::get_instance() { return Game::instance; }

// Most 
bool Game::init_game() {
	if (!window.init_window()) {
		DEBUG_PRINT("Failed to create window.");
		return false;
    }

	glfwSetCursorPosCallback(window.window, [](GLFWwindow* window, double d_x_pos, double d_y_pos) {
		// normalize to -1 : 1
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float x_pos = (float)d_x_pos / (float)width * 2.0f - 1.0f;
		float y_pos = (float)d_y_pos / (float)height * 2.0f - 1.0f;

		Game* game = Game::get_instance();
		glm::vec2 offset = glm::vec2(x_pos - game->mouse_pos.x, y_pos - game->mouse_pos.y);
		offset *= 180.0;

		game->ang.x += offset.x;	// yaw
		game->ang.y = std::clamp(game->ang.y + offset.y, -89.9f, 89.9f); // pitch
		game->mouse_pos = glm::vec2(x_pos, y_pos);

		game->update_transform();
		
	});


    return true;
}

void Game::update_transform() {
	glm::vec3 forward;
    forward.x = sin(glm::radians(ang.x)) * cos(glm::radians(ang.y));
    forward.y = cos(glm::radians(ang.x)) * cos(glm::radians(ang.y));
    forward.z = sin(glm::radians(ang.y));
	forward = glm::normalize(forward);

	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 0, 1)));
	glm::vec3 up = glm::cross(right, forward);

	window.set_uniform("rotation", glm::mat3x3(forward, right, up));
	window.set_uniform("eye_pos", pos);
}

void Game::run_main_loop() {

	while (!glfwWindowShouldClose(window.window)) {
		// MAIN LOOP HERE TODO: Create a loop function that actually contains all of the game data and all that.
		window.set_uniform("time", (float)glfwGetTime());

		//if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS)

	    glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLE_STRIP);
		{

			glVertex2d(-1.0, -1.0);
			glVertex2d(1.0, -1.0);
			glVertex2d(-1.0, 1.0);
			glVertex2d(1.0, 1.0);

		}
		glEnd();

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	
	}

	window.cleanup(); // Game is over TODO: Probably should add some sort of on end function maybe for cleaning or saving or something to that nature, just an idea.
}