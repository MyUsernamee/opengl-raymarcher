struct Object {
	int shape;
	int intersection_type; 
	#ifdef CPP 
	alignas(16) 
	#endif 
	mat4 model_matrix;
};

