#pragma once

#include <Shader.h>
#include <Texture.h>
#include <map>
#include <Material.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Acidrain {

using namespace std;

struct UniformDescriptor {
    string structureName;
    string memberName;  

    // TODO: implement memoization of uniform name calculation

    UniformDescriptor(const char* member) {
        structureName = "";
        memberName = member;
    }

    UniformDescriptor(const char* structure, const char* member) {
        structureName = structure;
        memberName = member;
    }

    std::string getName() {  
        if (structureName.empty())
            return memberName;

        std::stringstream ss(structureName);
        ss << "." << memberName;
        return ss.str();
    }

    std::string getName(int index) {
        std::stringstream ss;
        if (structureName.empty()) {
            ss << memberName << "[" << index << "]";
        } else {
            ss << structureName << "[" << index << "]." << memberName;
        }
        return ss.str();
    }
};

class Uniforms {
public:

    static UniformDescriptor ViewMatrix;
    static UniformDescriptor ProjectionMatrix;
    static UniformDescriptor ModelToWorldMatrix;
    static UniformDescriptor NormalMatrix;
    static UniformDescriptor WorldToLightMatrix;
    static UniformDescriptor LightProjectionMatrix;
    static UniformDescriptor ShadowBiasMatrix;
    static UniformDescriptor ShadowMatrix;
    static UniformDescriptor DemoPartNormalizedTime;
    static UniformDescriptor LightPositionInEyeSpace;
    static UniformDescriptor MaterialAmbientColor;
    static UniformDescriptor MaterialDiffuseColor;
    static UniformDescriptor MaterialSpecularColor;
    static UniformDescriptor MaterialShininess;
    static UniformDescriptor DiffuseMap;
    static UniformDescriptor ShadowMap;
    static UniformDescriptor ShadowMap1;
    static UniformDescriptor ShadowMap2;
    static UniformDescriptor ShadowMap3;
    static UniformDescriptor ShadowMap4;
    static UniformDescriptor NumberOfLights;
    static UniformDescriptor LightPosition;
    static UniformDescriptor LightTarget;
    static UniformDescriptor LightAmbient;
    static UniformDescriptor LightDiffuse;
    static UniformDescriptor LightSpecular;
    static UniformDescriptor LightSpotCutoff;
    static UniformDescriptor LightSpotExponent;
    static UniformDescriptor LightAttenuation;
};

enum class ShaderConstantType {
    Vec3,
    Vec4,
    Integer,
    Float,
    Mat3,
    Mat4
};

class ShaderConstantValue {
public:

    ShaderConstantType type;

    glm::vec3 vec3Val;
    glm::vec4 vec4Val;
    glm::mat3 mat3Val;
    glm::mat4 mat4Val;
    int intVal;
    float floatVal;

    ShaderConstantValue(const ShaderConstantValue& rhs) {
        type = rhs.type;
        vec3Val = rhs.vec3Val;
        vec4Val = rhs.vec4Val;
        mat3Val = rhs.mat3Val;
        mat4Val = rhs.mat4Val;
        intVal = rhs.intVal;
        floatVal = rhs.floatVal;
    }

    ShaderConstantValue(glm::vec3& value) {
        vec3Val = value;
        type = ShaderConstantType::Vec3;
    }

    ShaderConstantValue(glm::vec4& value) {
        vec4Val = value;
        type = ShaderConstantType::Vec4;
    }

    ShaderConstantValue(glm::mat3& value) {
        mat3Val = value;
        type = ShaderConstantType::Mat3;
    }

    ShaderConstantValue(glm::mat4& value) {
        mat4Val = value;
        type = ShaderConstantType::Mat4;
    }

    ShaderConstantValue(int value) {
        intVal = value;
        type = ShaderConstantType::Integer;
    }

    ShaderConstantValue(float value) {
        floatVal = value;
        type = ShaderConstantType::Float;
    }

    void applyTo(Shader& shader, const char* uniformName) {
        switch (type) {
            case ShaderConstantType::Vec3:
                shader.setVec3Uniform(glm::value_ptr(vec3Val), uniformName);
                break;
            case ShaderConstantType::Vec4:
                shader.setVec4Uniform(glm::value_ptr(vec4Val), uniformName);
                break;
            case ShaderConstantType::Mat3:
                shader.setMatrix3Uniform(glm::value_ptr(mat3Val), uniformName);
                break;
            case ShaderConstantType::Mat4:
                shader.setMatrix4Uniform(glm::value_ptr(mat4Val), uniformName);
                break;
            case ShaderConstantType::Integer:
                shader.setIntUniform(intVal, uniformName);
                break;
            case ShaderConstantType::Float:
                shader.setFloatUniform(floatVal, uniformName);
                break;
        }
    }
};


class ShaderConstants {
public:

    void set(UniformDescriptor descriptor, glm::mat3 value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::mat3 value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::mat4 value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::mat4 value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::vec3 value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::vec3 value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::vec4 value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, glm::vec4 value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, int value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, int value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, float value) {
        constants[descriptor.getName()] = make_shared<ShaderConstantValue>(value);
    }

    void set(UniformDescriptor descriptor, float value, int index) {
        constants[descriptor.getName(index)] = make_shared<ShaderConstantValue>(value);
    }

    void applyTo(Shader& shader) {
        for (auto& kv : constants) {
            kv.second->applyTo(shader, kv.first.c_str());
        }
    }

private:
    map<string, std::shared_ptr<ShaderConstantValue>> constants;
};

/*
enum class ShaderConstantType {
    ViewMatrix,
    ProjectionMatrix,
    ModelToWorldMatrix,
    NormalMatrix,
    WorldToLightMatrix,
    LightProjectionMatrix,
    ShadowBiasMatrix,
    ShadowMatrix,
    DemoPartNormalizedTime,
    LightPositionInEyeSpace,
    MaterialAmbientColor,
    MaterialDiffuseColor,
    MaterialSpecularColor,
    MaterialShininess,
    ShadowMap,
    NumberOfLights
};

class ShaderConstantNameResolver {
public:

    static const char* nameFor(const ShaderConstantType& type);
    static const char* nameFor(const TextureRole& type);
};

union ShaderConstantValue {
    glm::vec2 vec2Val;
    glm::vec3 vec3Val;
    glm::vec4 vec4Val;
    glm::mat3 mat3Val;
    glm::mat4 mat4Val;
    bool boolVal;
    int intVal;
    float floatVal;

    ShaderConstantValue() {
        this->floatVal = 0;
    };

    ShaderConstantValue(const ShaderConstantValue& lhs) {
        this->mat4Val = lhs.mat4Val;
    }

    ShaderConstantValue& operator=(const ShaderConstantValue& lhs) {
        this->mat4Val = lhs.mat4Val;
        return *this;
    };
};

struct ShaderConstants {
    std::map<ShaderConstantType, ShaderConstantValue> constants;

    void add(ShaderConstantType type, float value);
    void add(ShaderConstantType type, int value);
    void add(ShaderConstantType type, const glm::vec3& value);
    void add(ShaderConstantType type, const glm::vec4& value);
    void add(ShaderConstantType type, const glm::mat4& value);
    void add(ShaderConstantType type, const glm::mat3& value);
    void set(Shader& shader);
};
*/

} // namespace Acidrain