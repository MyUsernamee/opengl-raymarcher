#pragma once

#include <glad/glad.h>

class FrameBuffer {

private:
	GLuint frame_buffer_id;
	GLuint frame_buffer_texture;

public:
	FrameBuffer();
	FrameBuffer(int width, int height, GLenum interpolation_type);

	void use(); //Draw to
	void draw(); // Draw texture to screen

	GLuint get_texture();
	
};