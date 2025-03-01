#include "shader_compiler.h"

ShaderCompiler::ShaderCompiler() {
	program_id = 0;
}

void ShaderCompiler::add_shader(GLenum shader_type, const char* source) {
	shader_sources.push_back(Shader(shader_type, source));
}

bool ShaderCompiler::compile() {

	auto program = glCreateProgram();
	
	for (Shader shader_source: shader_sources) {

		GLuint shader = glCreateShader(shader_source.shader_type);
		glShaderSource(shader, 1, &shader_source.source, NULL);

		// Actually compile them
		glCompileShader(shader);

		int did_compile = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &did_compile); // Get if the shader compiled
		if (!did_compile) {
			// Get the length of error info.
			int length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			// Get the actual info
			std::vector<GLchar> info(length);
			glGetShaderInfoLog(shader, length, &length, &info[0]);
			info.push_back(0); // Add null terminator

			DEBUG_PRINT("Error compiling shader: %s", &info[0]);
			return false;
		}

		glAttachShader(program, shader);

		shader_ids.push_back(shader);
	}

	// Create the actual program and save it
	glLinkProgram(program);
	program_id = program;

	compiled = true;

	return true;
}

void ShaderCompiler::use() {
	
	if (!compiled) {
		DEBUG_PRINT("Attemping to use uncompiled shader program.");
		return;
	}

	if (in_use)
		return;

	glUseProgram(program_id);
	in_use = true;
}

ShaderCompiler::~ShaderCompiler() { 
	if(!compiled) // Causes segmentation fault, perhaps cleaning is already being handled?
		return;
}

#define COMMA ,
#define uniform_function_definition(T, params, ...)                  \
void ShaderCompiler::setUniform##T(const char* name, params) {         \
    if (!in_use)                                                       \
        return;                                                        \
    GLint location = glGetUniformLocation(program_id, name);           \
    setUniform##T(location, __VA_ARGS__);                              \
}                                                                      \
                                                                       \
void ShaderCompiler::setUniform##T(GLint location, params) {           \
    if (!in_use)                                                       \
        return;                                                        \
    glUniform##T(location, __VA_ARGS__);                               \
}

uniform_function_definition(1f, float value, value)
uniform_function_definition(2f, float v0 COMMA float v1, v0 COMMA v1)
uniform_function_definition(3f, float v0 COMMA float v1 COMMA float v2, v0 COMMA v1 COMMA v2)
uniform_function_definition(4f, float v0 COMMA float v1 COMMA float v2 COMMA float v3, v0 COMMA v1 COMMA v2 COMMA v3)

uniform_function_definition(1i, int value, value)
uniform_function_definition(2i, int v0 COMMA int v1, v0 COMMA v1)
uniform_function_definition(3i, int v0 COMMA int v1 COMMA int v2, v0 COMMA v1 COMMA v2)
uniform_function_definition(4i, int v0 COMMA int v1 COMMA int v2 COMMA int v3, v0 COMMA v1 COMMA v2 COMMA v3)

uniform_function_definition(1ui, unsigned int value, value)
uniform_function_definition(2ui, unsigned int v0 COMMA unsigned int v1, v0 COMMA v1)
uniform_function_definition(3ui, unsigned int v0 COMMA unsigned int v1 COMMA unsigned int v2, v0 COMMA v1 COMMA v2)
uniform_function_definition(4ui, unsigned int v0 COMMA unsigned int v1 COMMA unsigned int v2 COMMA unsigned int v3, v0 COMMA v1 COMMA v2 COMMA v3)

uniform_function_definition(Matrix2fv, GLsizei count COMMA GLboolean transpose COMMA const float* matrix, count COMMA transpose COMMA matrix)
uniform_function_definition(Matrix3fv, GLsizei count COMMA GLboolean transpose COMMA const float* matrix, count COMMA transpose COMMA matrix)
uniform_function_definition(Matrix4fv, GLsizei count COMMA GLboolean transpose COMMA const float* matrix, count COMMA transpose COMMA matrix)
