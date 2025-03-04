#ifndef MARCH_H
#define MARCH_H

#define EPSILON 1e-6f
#define EPSILON_FALL_OFF_POWER 10.0f
#define EPSILON_FALL_OFF_SCALE 8.0f

struct Material {
	vec3 color;
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

float box_sdf(vec3 pos) {
	vec3 d = abs(pos) - vec3(1.0);
	return min(max(d.x, max(d.y, d.z)), 0.0f) + length(max(d, 0.0f));
}

float sdf_type(int sdf_type, vec3 pos) {
	switch(sdf_type) {
		case SDF_SPHERE:
			return sphere_sdf(pos);
		case SDF_BOX:
			return box_sdf(pos);
		case SDF_MANDLEBROT:
			return mandlebrot_sdf(pos);
		// Add more cases as needed for other SDF types
		default:
			return 0.0; // Default case if no SDF type matches
	}
}

float intersect(int intersection_type, float a, float b) {
	switch(intersection_type) {
		case INTERSECTION_UNION:
			return min(a, b);
		case INTERSECTION_SUBTRACT:
			return max(-a, b);
		case INTERSECTION_INTERSECTION:
			return max(a, b);
		default:
			return 0.0; // Default case if no intersection type matches
	}
}

float sdf(vec3 pos) {

	float current_distance = sdf_type(objects[0].shape, vec3(inverse(objects[0].model_matrix) * vec4(pos, 1.0)));

	for (int i = 1; i < object_count; i++) {
		current_distance = intersect(objects[i].intersection_type, sdf_type(objects[i].shape, vec3(inverse(objects[i].model_matrix) * vec4(pos, 1.0))), current_distance);
	}

	return current_distance;
}

struct MarchData {
    vec3 position;
    float t;
    int steps;
};

MarchData march(vec3 start, vec3 end) {

	vec3 position = start;
	vec3 direction = normalize(end - start);
	float d = sdf(start);
    float t = 0.0;
    int it;
	float dist = distance(position, end);

    MarchData data;

	// TODO: AVOID DYNAMIC LOOPS!!!

	for (it = 0; it < 1e10 && d < dist; it++) {

		d = sdf(position);
		t += d;
		position += direction * d;

		if (d < EPSILON * min(pow(EPSILON_FALL_OFF_POWER, t * EPSILON_FALL_OFF_SCALE), dist)) {
            data.position = position;
            data.t = t;
            data.steps = it;
			return data;
		}
	}
    data.position = end;
    data.t = t;
    data.steps = it;

	return data;
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