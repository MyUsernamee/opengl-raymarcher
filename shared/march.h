#ifndef MARCH_H
#define MARCH_H

#define EPSILON 0.0001f

struct Material {
	vec3 color;
};

struct Object {
	
	Material material;
	mat4 model_matrix;
};

float sdf(vec3 pos) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    const int iterations = 16;
    const float power = 8.0;
    
    for (int i = 0; i < iterations; i++) {
        r = length(z);
        if (r > 2.0) break;
        
        // convert to spherical coordinates
        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
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
#endif