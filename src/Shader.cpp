#include <Shader.h>

namespace Acidrain {

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

    glUseProgram(programId);
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

void Shader::setMatrixUniform(float* matrix, const char* uniformName) {
    GLint location = glGetUniformLocation(programId, uniformName);
    if (location != -1)
        glUniformMatrix4fv(location, 1, false, matrix);
}

void Shader::setVec3Uniform(float* value, const char* uniformName) {
    GLint location = glGetUniformLocation(programId, uniformName);
    if (location != -1)
        glUniform3fv(location, 1, value);
}

void Shader::setVec4Uniform(float* value, const char* uniformName) {
    GLint location = glGetUniformLocation(programId, uniformName);
    if (location != -1)
        glUniform4fv(location, 1, value);
}

void Shader::setIntUniform(int value, const char* uniformName) {
    GLint location = glGetUniformLocation(programId, uniformName);
    if (location != -1)
        glUniform1i(location, value);
}

void Shader::setFloatUniform(float value, const char* uniformName) {
    GLint location = glGetUniformLocation(programId, uniformName);
    if (location != -1)
        glUniform1f(location, value);
}

} // namespace Acidrain

