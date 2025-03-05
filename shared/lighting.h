#define LIGHT_POS vec3(-1, -1, 1) * 10.0

// soft shadow technique from https://iquilezles.org/articles/rmshadows/ 
// TODO: use from https://www.shadertoy.com/view/lsKcDD instead, as this one fixes sharp corners
float shadow(in vec3 pos, float min_t, float max_t, float k) {
	vec3 light_dir = normalize(LIGHT_POS - pos);
    float res = 1.0;
    for(int i = 0; i < 256 && min_t < max_t; i++) {
        float h = sdf(pos + light_dir * min_t);
        if(h < EPSILON * 10.0) return 0.0;

        res = min(res, k * h / min_t);
        min_t += h;
    }
    return res;
}

vec3 phong(vec3 pos, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(LIGHT_POS - pos);
    vec3 reflect_dir = reflect(light_dir, normal);
    
    float ambient = 0.0;
    float specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    
    return vec3(ambient + specular);
}