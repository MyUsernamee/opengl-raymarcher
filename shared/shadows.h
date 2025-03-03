
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