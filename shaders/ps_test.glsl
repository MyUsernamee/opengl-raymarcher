#version 410 core
in vec2 screen_uv;
out vec4 FragColor;

uniform float time;
uniform mat3x3 rotation;
uniform vec3 eye_pos;

uniform float aspect_ratio; // width / height

#include <object.h>

uniform int object_count;
layout(std140) uniform ObjectBlock {
    Object objects[MAX_OBJECTS];
};

#include <march.h>
#include <lighting.h>

void main() {


	vec3 ray_direction = (rotation * normalize(vec3(1.0, (screen_uv.x) * aspect_ratio, screen_uv.y)));
	//FragColor = vec4(ray_direction, 1.0);

	vec3 start = eye_pos + ray_direction * 0.00; 
	vec3 end = eye_pos + ray_direction * 1000.0;

	MarchData march_data = march(start, end);
	
	vec3 hit_pos = march_data.position;
	if (hit_pos != end) {
		vec3 hit_normal = get_normal(hit_pos);
		float shadow = shadow(hit_pos, 0.01, 10, 8) *1 + 0.0;
		FragColor = vec4(phong(hit_pos, hit_normal, ray_direction) * shadow, 1.0);
	} else {
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	return;
}