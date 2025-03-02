#version 440
in vec2 screen_uv;
out vec4 FragColor;

uniform float time;
uniform mat3x3 rotation;
uniform vec3 eye_pos;

uniform float aspect_ratio; // width / height

#define EPSILON 0.0001
float sdf(vec3 pos) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    const int iterations = 32;
    const float power = 8.0;
    
    for (int i = 0; i < iterations; i++) {
        r = length(z);
        if (r > 2.0) break;
        
        // convert to spherical coordinates
        float theta = acos(z.z / r) + time / 100;
        float phi = atan(z.y, z.x) + time / 100;
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

vec3 march(vec3 start, vec3 end) {

	vec3 position = start;
	vec3 direction = normalize(end-start);
	float d = sdf(start);
	float dist = distance(position, end);

	// TODO: AVOID DYNAMIC LOOPS!!!
	while (d < dist)
	{

		d = sdf(position);
		position += direction * d;

		if (d < EPSILON) {
			return position;
		}

	}

	return end;

}

// extracts normal at position using tetrahedral sampling 
vec3 get_normal(vec3 pos) {
    vec3 d1 = vec3( 1, -1, -1);
    vec3 d2 = vec3(-1, -1,  1);
    vec3 d3 = vec3(-1,  1, -1);
    vec3 d4 = vec3( 1,  1,  1);

    float s1 = sdf(pos + EPSILON * d1);
    float s2 = sdf(pos + EPSILON * d2);
    float s3 = sdf(pos + EPSILON * d3);
    float s4 = sdf(pos + EPSILON * d4);

    return normalize(d1 * s1 + d2 * s2 + d3 * s3 + d4 * s4);
}

// soft shadow technique from https://iquilezles.org/articles/rmshadows/ 
// TODO: use from https://www.shadertoy.com/view/lsKcDD instead, as this one fixes sharp corners
float shadow(in vec3 pos, in vec3 dir, float min_t, float max_t, float k) {
    float res = 1.0;
    for(int i = 0; i < 256 && min_t < max_t; i++) {
        float h = sdf(pos + dir * min_t);
        if(h < EPSILON) return 0.0;

        res = min(res, k * h / min_t);
        min_t += h;
    }
    return res;
}

void main() {

	vec3 ray_direction = (rotation * normalize(vec3(1.0, (screen_uv.x) * aspect_ratio, screen_uv.y)));
	//FragColor = vec4(ray_direction, 1.0);

	vec3 start = eye_pos + ray_direction * 0.003; 
	vec3 end = eye_pos + ray_direction * 10.0;

	vec3 hit_pos = march(start, end);
	if (hit_pos != end) {
		float shadow = shadow(hit_pos, normalize(vec3(-1, -1, 1)), 0.01, 1, 8);
		FragColor = vec4(get_normal(hit_pos) * 0.5 + 0.5, 1.0) * shadow;
	} else {
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}