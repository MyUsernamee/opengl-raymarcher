#version 440
in vec2 screen_uv;
out vec4 FragColor;

#define MAX 9999999999999.0
#define SAFE_DISTANCE 0.0001

float sdf(vec3 position) {
	return length((position - vec3(0.0))) - 1.0;
}

bool march(vec3 start, vec3 end) {

	vec3 position = start;
	vec3 direction = normalize(end-start);
	float d = sdf(start);

	while (d < distance(position, end))
	{

		d = sdf(position);
		position += direction * d;

		if (d < SAFE_DISTANCE) {
			return true;
		}

	}

	return false;

}

void main() {

	vec3 ray_direction = normalize(vec3(1.0, screen_uv.x, screen_uv.y));
	vec3 start = vec3(-4.0, 0.0, 0.0);

	FragColor = march(start, start + ray_direction * 10.0) ? vec4(1.0, 1.0, 1.0f, 1.0f) : vec4(0.0);
}