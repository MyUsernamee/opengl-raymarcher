#version 440
in vec2 screen_uv;
out vec4 FragColor;

void main() {
	FragColor = vec4(screen_uv, 0.2f, 1.0f);
}