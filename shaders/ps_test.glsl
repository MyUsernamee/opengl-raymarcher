#version 440
in vec2 screen_uv;
out vec4 FragColor;

uniform float time;

#define MAX 9999999999999.0
#define SAFE_DISTANCE 0.0001
float sdf(vec3 pos) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    const int iterations = 256;
    const float power = 8.0;
    
    for (int i = 0; i < iterations; i++) {
        r = length(z);
        if (r > 2.0) break;
        
        // convert to spherical coordinates
        float theta = acos(z.z / r) + time;
        float phi = atan(z.y, z.x) + time;
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

	vec3 ray_direction = normalize(vec3(1.0, screen_uv.x, screen_uv.y));
	vec3 start = vec3(-2.0, 0.0, 0.0);
	int steps = march(start, start + ray_direction * 10.0);
	FragColor = vec4(1.0 / pow(steps, 0.25));

}