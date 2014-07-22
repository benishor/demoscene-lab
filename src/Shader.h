#pragma once

#include <GLheaders.h>

class Shader {
public:

    Shader(const char* vertexShaderContent, const char* pixelShaderContent);
    virtual ~Shader();

    void use();
    void unuse();

private:
	GLuint programId;
	GLuint shaderIds[2];
};