
#define MAX_OBJECTS 32

#define INTERSECTION_UNION 0
#define INTERSECTION_SUBTRACT 1
#define INTERSECTION_INTERSECTION 2
#define INTERSECTION_UNION_SMOOTH 3
#define INTERSECTION_SUBTRACTION_SMOOTH 4

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
static Object objects[MAX_OBJECTS];
static size_t object_count = 0;

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

void update_gpu_objects() {
	ray_marcher_program->set_uniform_buffer("ObjectBlock", (void *)objects,
				  object_count * sizeof(Object));
	ray_marcher_program->set_uniform("object_count", (int)object_count);
}

Object *get_object(int index) { return objects+index; }
void set_object(int index, Object object) {
	if (index >= object_count)
		return;
	objects[index] = object;
}

void add_object(Object object) { 
	object_count++;
	set_object(object_count - 1, object);
}

#endif