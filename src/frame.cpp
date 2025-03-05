#include "frame_buffer.h"

FrameBuffer::FrameBuffer() {

};

FrameBuffer::FrameBuffer(int width, int height, GLenum interpolation_type) {

	glGenFramebuffers(1, &frame_buffer_id);
	glGenTextures(1, &frame_buffer_texture);

	glBindTexture(GL_TEXTURE_2D, frame_buffer_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation_type);

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer_texture, 0);	

}

void FrameBuffer::use() {

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

}

void FrameBuffer::draw() {
	glBindTexture(GL_TEXTURE_2D, frame_buffer_texture);
}

GLuint FrameBuffer::get_texture() {
	return frame_buffer_texture;
}