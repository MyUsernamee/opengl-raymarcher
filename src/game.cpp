
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

    ang = glm::vec3(0.0);
	camera_forward = glm::vec3(1.0, 0.0, 0.0);

    glfwSetCursorPosCallback(
		window.window, [](GLFWwindow *window, double d_x_pos, double d_y_pos) {
			int width, height;
			Game *game = Game::get_instance();

			glfwGetWindowSize(window, &width, &height);

			// normalize to -1 : 1
			float x_pos = (float)d_x_pos / (float)width * 2.0f - 1.0f;
			float y_pos = (float)d_y_pos / (float)height * 2.0f - 1.0f;

			glm::vec2 mouse_delta =
				glm::vec2(x_pos - game->mouse_pos.x, y_pos - game->mouse_pos.y);
			mouse_delta *= 180.0;

			game->ang.x += mouse_delta.x; // yaw
			game->ang.y =
				std::clamp(game->ang.y + mouse_delta.y, -89.9f, 89.9f); // pitch

			game->mouse_pos = glm::vec2(x_pos, y_pos);

	});

    glfwSetKeyCallback(window.window,
		       [](GLFWwindow *window, int key, int scancode, int action,
			  int _) { 
			auto game = Game::get_instance();

			game->key_down_map.insert_or_assign(
			    key, action != GLFW_RELEASE);

	});

    return true;
}

void Game::update_transform() {

	camera_forward.x = sin(glm::radians(ang.x)) * cos(glm::radians(ang.y));
    camera_forward.y = cos(glm::radians(ang.x)) * cos(glm::radians(ang.y));
    camera_forward.z = sin(glm::radians(ang.y));
	camera_forward = glm::normalize(camera_forward);
	glm::vec3 right = glm::normalize(glm::cross(camera_forward, glm::vec3(0, 0, 1)));
	glm::vec3 up = glm::cross(right, camera_forward);

	window.set_uniform("rotation", glm::mat3x3(camera_forward, right, up));
	window.set_uniform("eye_pos", pos);
}

bool Game::is_key_down(int key) { return key_down_map.count(key) ?  key_down_map.at(key) : false; }

void Game::run_main_loop() {
	
	while (!window.should_close()) {
		window.set_uniform("time", (float)glfwGetTime());
		window.set_uniform("aspect_ratio", (float)window.width() / (float)window.height()); // TODO: This is run every frame but like mentioned
		update_transform();
		// in the getters for width and height there is probably a hook
		// we can use.

		//if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS)

		if(is_key_down(GLFW_KEY_W)) {
			pos += camera_forward * 0.01f;
		}
		
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