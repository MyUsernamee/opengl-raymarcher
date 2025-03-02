#version 440
in vec2 screen_uv;
out vec4 FragColor;

uniform float time;
uniform mat3x3 rotation;
uniform vec3 eye_pos;

uniform float aspect_ratio; // width / height

#define MAX 9999999999999.0
#define SAFE_DISTANCE 0.000001
float sdf(vec3 pos) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    const int iterations = 64;
    const float power = 8.0;
    
    for (int i = 0; i < iterations; i++) {
        r = length(z);
        if (r > 2.0) break;
        
        // convert to spherical coordinates
        float theta = acos(z.z / r) + time / 10;
        float phi = atan(z.y, z.x) + time / 10;
        dr = pow(r, power - 1.0) * power * dr + 1.0;
        
        // scale and rotate the point
        float zr = pow(r, power);
        theta *= power;
        phi   *= power;
        z = zr * vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
        z += pos;
    }
    
    return 0.5 * log(r) * r / dr;
}

int march(vec3 start, vec3 end) {

	vec3 position = start;
	vec3 direction = normalize(end-start);
	float d = sdf(start);
	int steps = 0;


	while (d < distance(position, end))
	{

		d = sdf(position);
		position += direction * d;
		steps += 1;

		if (d < SAFE_DISTANCE) {
			return steps;
		}

	}

	return 0;

}

void main() {

	vec3 ray_direction = (rotation * normalize(vec3(1.0, screen_uv.x * 2.0 - 1.0, 1.0 - 2.0 * screen_uv.y)));
	//FragColor = vec4(ray_direction, 1.0);
	int steps = march(eye_pos, eye_pos + ray_direction * 10.0);
	FragColor = vec4(1.0 / pow(steps, 0.25));

}