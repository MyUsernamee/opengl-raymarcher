#pragma once

#include "debug.h"
#include "glad/glad.h"

#include <vector>

#define uniform_function(T, ...)                                               \
  void setUniform##T(const char *name, __VA_ARGS__);\
   void setUniform##T(GLint location, __VA_ARGS__);

struct Shader {
	GLenum shader_type;
	const char* source;

	Shader(GLenum shader_type, const char* source) {
		this->shader_type = shader_type;
		this->source = source;
	}
};
struct ShaderCompiler {
    // Constructors & Destructor
    ShaderCompiler();
    ~ShaderCompiler();

    // Members
    GLuint program_id;
    std::vector<GLuint> shader_ids;
    std::vector<Shader> shader_sources;
    bool compiled = false;
    bool in_use = false;

    // Float uniform declarations
    uniform_function(1f, float v0)
    uniform_function(2f, float v0, float v1)
    uniform_function(3f, float v0, float v1, float v2)
    uniform_function(4f, float v0, float v1, float v2, float v3)

    // Integer uniform declarations
    uniform_function(1i, int value)
    uniform_function(2i, int v0, int v1)
    uniform_function(3i, int v0, int v1, int v2)
    uniform_function(4i, int v0, int v1, int v2, int v3)

    // Unsigned integer uniform declarations
    uniform_function(1ui, unsigned int value)
    uniform_function(2ui, unsigned int v0, unsigned int v1)
    uniform_function(3ui, unsigned int v0, unsigned int v1, unsigned int v2)
    uniform_function(4ui, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)

    // Matrix uniform declarations
    uniform_function(Matrix2fv, GLsizei count, GLboolean transpose, const float* matrix )
    uniform_function(Matrix3fv, GLsizei count, GLboolean transpose, const float* matrix )
    uniform_function(Matrix4fv, GLsizei count, GLboolean transpose, const float* matrix )

    // Other shader functions
    void add_shader(GLenum type, const char* source);
    bool compile();
    void use();
};
