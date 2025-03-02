#pragma once

#include "window.h"
#include <algorithm> // std::clamp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// This is a singleton class that store the game state
class Game {

public:
	// Creates and initializes the game. If the game instance already exists it will just return that.
	Game();

	static Game *get_instance();
	glm::vec3 pos = glm::vec3(-1.2, 0.0, 0.0);
	glm::vec2 mouse_pos;
	glm::vec2 ang;
	glm::vec3 camera_forward;
	glm::mat4x4 view;

    std::map<int, bool> key_down_map;

	bool init_game();
	void update_transform();
	void run_main_loop();

	bool is_key_down(int key);

private:
	static Game *instance;
	Window window;
};