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

	glUseProgram(program_id); 
}

ShaderCompiler::~ShaderCompiler() { 
	glDeleteProgram(program_id); 
	for (auto shader_id: shader_ids) {
		glDeleteShader(shader_id);
	}
}
