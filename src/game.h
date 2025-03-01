#pragma once

#include "window.h"

// This is a singleton class that store the game state
class Game {

public:
	// Creates and initializes the game. If the game instance already exists it will just return that.
	Game();

	static Game *get_instance();

	bool init_game();
	void run_main_loop();

private:

	static Game *instance;
	Window window;
};