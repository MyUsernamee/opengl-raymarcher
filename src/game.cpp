
#include "game.h"

Game::Game() {

	if (!Game::get_instance()) {
		Game::instance = this;
	}

	window = new Window();

}

Game *Game::get_instance() { return Game::instance; }