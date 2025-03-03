#ifndef MARCH_H
#define MARCH_H

#define EPSILON 1e-6f

struct Material {
	vec3 color;
};

struct Object {
	int shape;
	Material material;
	mat4 model_matrix;
};

float mandlebrot_sdf(vec3 pos) {
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

float julia_sdf(vec3 p) {
    // Convert 3D point to a 4D quaternion (with w = 0)
    vec4 z = vec4(p, 0.0);
    // Constant c controls the shape of the Julia set.
    vec4 c = vec4(0.5, 0.7, 0.35, 0.0);
    
    float dr = 2.0;  // derivative factor
    float r = 0.0;
    const int ITERATIONS = 10;
    
    for (int i = 0; i < ITERATIONS; i++) {
        r = length(z);
        if (r > 4.0) break;
        
        // Update derivative: dr = 2 * |z| * dr + 1
        dr = 2.0 * r * dr + 1.0;
        
        // Quaternion square: for z = (x,y,z,w),
        // z^2 = (x^2 - y^2 - z^2 - w^2, 2*x*y, 2*x*z, 2*x*w)
        vec4 z_new;
        z_new.x = z.x*z.x - z.y*z.y - z.z*z.z - z.w*z.w;
        z_new.y = 2.0 * z.x * z.y;
        z_new.z = 2.0 * z.x * z.z;
        z_new.w = 2.0 * z.x * z.w;
        z = z_new + c;
    }
    
    // Return the distance estimate.
    return 0.5 * log(r) * r / dr;
}

float sphere_sdf(vec3 pos) { return length(pos) - 1.0; }

// Smooth maximum using polynomial interpolation.
float smoothMaxPoly(float a, float b, float k) {
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    return mix(b, a, h) + k * h * (1.0 - h);
}

// Smooth minimum using polynomial interpolation.
float smoothMinPoly(float a, float b, float k) {
    float h = clamp(0.5 - 0.5 * (b - a) / k, 0.0, 1.0);
    return mix(b, a, h) - k * h * (1.0 - h);
}


float sdf(vec3 pos) {return mandlebrot_sdf(pos); }

vec3 march(vec3 start, vec3 end) {

	vec3 position = start;
	vec3 direction = normalize(end - start);
	float d = sdf(start);
	float t = 0.0;
	float dist = distance(position, end);

	// TODO: AVOID DYNAMIC LOOPS!!!

	for (int it = 0; it < 1e10 && d < dist; it++) {

		d = sdf(position);
		t += d;
		position += direction * d;

		if (d < EPSILON * min(pow(10.0f, t * 4.0f), dist)) {
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