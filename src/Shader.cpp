#include <Shader.h>
#include <string>
#include <iostream>
#include <vector>

namespace Acidrain {

void checkCompileError(GLuint shaderId) {
    GLint isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        char errorLog[maxLength];
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

        std::cout << "Shader error: " << errorLog << std::endl;

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(shaderId); //Don't leak the shader.
        return;
    }
}


Shader::Shader(const char* vertexShaderContent, const char* pixelShaderContent) {
    shaderIds[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderIds[0], 1, &vertexShaderContent, nullptr);
    glCompileShader(shaderIds[0]);
    checkCompileError(shaderIds[0]);

    shaderIds[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderIds[1], 1, &pixelShaderContent, nullptr);
    glCompileShader(shaderIds[1]);
    checkCompileError(shaderIds[1]);

    programId = glCreateProgram();
    glAttachShader(programId, shaderIds[0]);
    glAttachShader(programId, shaderIds[1]);

    // TODO Adrian: these should come from an external enumeration
    glBindAttribLocation(programId, 0, "position");
    glBindAttribLocation(programId, 1, "normal");
    glBindAttribLocation(programId, 2, "texCoords");

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


int Shader::getUniform(const char* uniformName) {
    if (uniformCache.count(uniformName) == 0) {
        uniformCache[uniformName] = glGetUniformLocation(programId, uniformName);
    }
    return uniformCache[uniformName];
}

void Shader::setMatrix3Uniform(float* matrix, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniformMatrix3fv(location, 1, false, matrix);
}

void Shader::setMatrix4Uniform(float* matrix, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniformMatrix4fv(location, 1, false, matrix);
}

void Shader::setVec3Uniform(float* value, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniform3fv(location, 1, value);
}

void Shader::setVec4Uniform(float* value, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniform4fv(location, 1, value);
}

void Shader::setIntUniform(int value, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniform1i(location, value);
}

void Shader::setFloatUniform(float value, const char* uniformName) {
    GLint location = getUniform(uniformName);
    if (location != -1)
        glUniform1f(location, value);
}

} // namespace Acidrain

