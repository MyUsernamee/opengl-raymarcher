#include "game.h"
#include <stdio.h>

int main() {
	Game game;
	if (!game.init_game()) return 1;

	game.run_main_loop();
	return 0;
}
