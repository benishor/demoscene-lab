#include <Shader.h>

Shader::Shader(const char* vertexShaderContent, const char* pixelShaderContent) {
	shaderIds[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderIds[0], 1, &vertexShaderContent, nullptr);
	glCompileShader(shaderIds[0]);

	shaderIds[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderIds[1], 1, &pixelShaderContent, nullptr);
	glCompileShader(shaderIds[1]);

	programId = glCreateProgram();
	glAttachShader(programId, shaderIds[0]);
	glAttachShader(programId, shaderIds[1]);
	glLinkProgram(programId);
}

Shader::~Shader() {
	glDetachShader(programId, shaderIds[0]);
	glDetachShader(programId, shaderIds[1]);

	glDeleteShader(shaderIds[0]);
	glDeleteShader(shaderIds[1]);

	glDeleteProgram(programId);
}

void Shader::use() {
	glUseProgram(programId);
}

void Shader::unuse() {
	glUseProgram(0);
}