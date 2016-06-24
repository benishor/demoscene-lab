#pragma once

#include <GLheaders.h>
#include <map>

namespace Acidrain {

    class Shader {
    public:

        Shader(const char* vertexShaderContent, const char* pixelShaderContent);
        virtual ~Shader();

        void use();
        void unuse();

        GLuint getId() const { return programId; }
        int getUniform(const char* uniformName);

        void setMatrix3Uniform(float* value, const char* uniformName);
        void setMatrix4Uniform(float* value, const char* uniformName);
        void setVec3Uniform(float* value, const char* uniformName);
        void setVec4Uniform(float* value, const char* uniformName);
        void setFloatUniform(float value, const char* uniformName);
        void setIntUniform(int value, const char* uniformName);

    private:
        GLuint programId;
        GLuint shaderIds[2];
        std::map<const char*, int> uniformCache;
    };

} // namespace Acidrain

