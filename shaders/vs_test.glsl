#version 440

layout (location = 0) in vec3 aPos;

out vec2 screen_uv;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	screen_uv = vec2(aPos.x, aPos.y);
}