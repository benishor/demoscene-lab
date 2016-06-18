#include <ShaderConstants.h>
#include <glm/gtc/type_ptr.hpp>

namespace Acidrain {

UniformDescriptor Uniforms::ViewMatrix("vmtx");
UniformDescriptor Uniforms::ProjectionMatrix("pmtx");
UniformDescriptor Uniforms::ModelToWorldMatrix("mmtx");
UniformDescriptor Uniforms::NormalMatrix("nmtx");
UniformDescriptor Uniforms::WorldToLightMatrix("wlmtx");
UniformDescriptor Uniforms::LightProjectionMatrix("lpmtx");
UniformDescriptor Uniforms::ShadowBiasMatrix("sbmtx");
UniformDescriptor Uniforms::ShadowMatrix("smtx");
UniformDescriptor Uniforms::DemoPartNormalizedTime("dptimen");
UniformDescriptor Uniforms::LightPositionInEyeSpace("lPosEye");
UniformDescriptor Uniforms::MaterialAmbientColor("matAmbientCol");
UniformDescriptor Uniforms::MaterialDiffuseColor("matDiffuseCol");
UniformDescriptor Uniforms::MaterialSpecularColor("matSpecularCol");
UniformDescriptor Uniforms::MaterialShininess("matShininess");
UniformDescriptor Uniforms::ShadowMap("shadowMap");
UniformDescriptor Uniforms::ShadowMap1("shadowMap1");
UniformDescriptor Uniforms::ShadowMap2("shadowMap2");
UniformDescriptor Uniforms::ShadowMap3("shadowMap3");
UniformDescriptor Uniforms::ShadowMap4("shadowMap4");
UniformDescriptor Uniforms::NumberOfLights("numLights");
UniformDescriptor Uniforms::LightPosition("light", "position");
UniformDescriptor Uniforms::LightTarget("light", "target");
UniformDescriptor Uniforms::LightAmbient("light", "ambient");
UniformDescriptor Uniforms::LightDiffuse("light", "diffuse");
UniformDescriptor Uniforms::LightSpecular("light", "specular");
UniformDescriptor Uniforms::LightSpotCutoff("light", "spotCutoff");
UniformDescriptor Uniforms::LightSpotExponent("light", "spotExponent");
UniformDescriptor Uniforms::LightAttenuation("light", "attenuation");

/*
const char* ShaderConstantNameResolver::nameFor(const ShaderConstantType& type) {
    switch (type) {
        case ShaderConstantType::ViewMatrix: return "vmtx";
        case ShaderConstantType::ProjectionMatrix: return "pmtx";
        case ShaderConstantType::ModelToWorldMatrix: return "mmtx";
        case ShaderConstantType::NormalMatrix: return "nmtx";
        case ShaderConstantType::DemoPartNormalizedTime: return "dptimen";
        case ShaderConstantType::WorldToLightMatrix: return "wlmtx";
        case ShaderConstantType::LightProjectionMatrix: return "lpmtx";
        case ShaderConstantType::ShadowBiasMatrix: return "sbmtx";
        case ShaderConstantType::ShadowMatrix: return "smtx";
        case ShaderConstantType::LightPositionInEyeSpace: return "lPosEye";
        case ShaderConstantType::MaterialAmbientColor: return "matAmbientCol";
        case ShaderConstantType::MaterialDiffuseColor: return "matDiffuseCol";
        case ShaderConstantType::MaterialSpecularColor: return "matSpecularCol";
        case ShaderConstantType::MaterialShininess: return "matShininess";
        case ShaderConstantType::ShadowMap: return "shadowMap";
        case ShaderConstantType::NumberOfLights: return "numLights";
        default: return "";
    }
}

const char* ShaderConstantNameResolver::nameFor(const TextureRole& type) {
    switch (type) {
        case TextureRole::Diffuse: return "mapDiffuse";
        case TextureRole::Specular: return "mapSpecular";
        case TextureRole::Normal: return "mapNormal";
        // TODO: remove Bump since we have normal map
        case TextureRole::Bump: return "mapBump";
        default: return "";
    }
}

void ShaderConstants::add(ShaderConstantType type, float value) {
    ShaderConstantValue val;
    val.floatVal = value;
    constants[type] = val;
}

void ShaderConstants::add(ShaderConstantType type, int value) {
    ShaderConstantValue val;
    val.intVal = value;
    constants[type] = val;
}

void ShaderConstants::add(ShaderConstantType type, const glm::vec3& value) {
    ShaderConstantValue val;
    val.vec3Val = value;
    constants[type] = val;
}

void ShaderConstants::add(ShaderConstantType type, const glm::vec4& value) {
    ShaderConstantValue val;
    val.vec4Val = value;
    constants[type] = val;
}

void ShaderConstants::add(ShaderConstantType type, const glm::mat3& value) {
    ShaderConstantValue val;
    val.mat3Val = value;
    constants[type] = val;
}

void ShaderConstants::add(ShaderConstantType type, const glm::mat4& value) {
    ShaderConstantValue val;
    val.mat4Val = value;
    constants[type] = val;
}


void ShaderConstants::set(Shader& shader) {
    for (auto& kv : constants) {

        const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);

        switch (kv.first) {
            case ShaderConstantType::NormalMatrix: {
                shader.setMatrix3Uniform(glm::value_ptr(kv.second.mat3Val), uniformName);
                break;
            }

            case ShaderConstantType::ViewMatrix:
            case ShaderConstantType::ProjectionMatrix:
            case ShaderConstantType::ModelToWorldMatrix:
            case ShaderConstantType::WorldToLightMatrix:
            case ShaderConstantType::LightProjectionMatrix:
            case ShaderConstantType::ShadowBiasMatrix:
            case ShaderConstantType::ShadowMatrix: {
                shader.setMatrix4Uniform(glm::value_ptr(kv.second.mat4Val), uniformName);
                break;
            }

            case ShaderConstantType::DemoPartNormalizedTime: {
                shader.setFloatUniform(kv.second.floatVal, uniformName);
                break;
            }

            case ShaderConstantType::MaterialShininess: 
            case ShaderConstantType::ShadowMap: 
            case ShaderConstantType::NumberOfLights: {
                shader.setIntUniform(kv.second.intVal, uniformName);
                break;
            }

            case ShaderConstantType::LightPositionInEyeSpace: {
                shader.setVec3Uniform(glm::value_ptr(kv.second.vec3Val), uniformName);
                break;
            }

            case ShaderConstantType::MaterialAmbientColor:
            case ShaderConstantType::MaterialDiffuseColor:
            case ShaderConstantType::MaterialSpecularColor: {
                shader.setVec4Uniform(glm::value_ptr(kv.second.vec4Val), uniformName);
                break;
            }

            default:
                break;
        }
    }
}
*/


} // namespace Acidrain

