
#include "game.h"

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

    return true;
}

void Game::run_main_loop() {

	while (!glfwWindowShouldClose(window.window)) {
	    glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLE_STRIP);
		{

			glVertex2d(-1.0, -1.0);
			glVertex2d(1.0, -1.0);
			glVertex2d(-1.0, 1.0);
			glVertex2d(1.0, 1.0);

		}
		glEnd();

		// MAIN LOOP HERE TODO: Create a loop function that actually contains all of the game data and all that.
		window.set_uniform("time", (float)glfwGetTime());

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	
	}

	window.cleanup(); // Game is over TODO: Probably should add some sort of on end function maybe for cleaning or saving or something to that nature, just an idea.
}