
#define INTERSECTION_UNION 0
#define INTERSECTION_SUBTRACT 1
#define INTERSECTION_INTERSECTION 2

#define SDF_SPHERE 0
#define SDF_BOX 1
#define SDF_MANDLEBROT 2


struct Object {
	int shape;
	int intersection_type; 
	#ifdef CPP 
	alignas(16) 
	#endif 
	mat4 model_matrix;
};

#ifdef CPP

// Utility functions that won't be used on the gpu

Object create_object(int shape, int intersection_type=INTERSECTION_UNION, vec3 position = vec3(0.0), mat3 rotation = mat3(1.0), float scale=1.0f) 
{

	mat4 model_matrix = mat4(rotation);
	model_matrix *= scale;
	model_matrix[3] = vec4(position, 1.0);

	return Object{
		shape,
		intersection_type,
		model_matrix
	};

}

#endif