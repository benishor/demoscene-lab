#include <ShaderConstants.h>

namespace Acidrain {

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
        default: return "";
    }
}

const char* ShaderConstantNameResolver::nameFor(const TextureRole& type) {
    switch (type) {
        case TextureRole::Diffuse: return "mapDiffuse";
        case TextureRole::Specular: return "mapSpecular";
        case TextureRole::Normal: return "mapNormal";
        case TextureRole::Bump: return "mapBump";
        default: return "";
    }
}

void ShaderConstants::add(ShaderConstantType type, void* value) {
    constants[type] = value;
}

void ShaderConstants::set(Shader& shader) {
    for (auto& kv : constants) {

        const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);

        switch (kv.first) {
            case ShaderConstantType::NormalMatrix: {
                shader.setMatrix3Uniform(reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }

            case ShaderConstantType::ViewMatrix:
            case ShaderConstantType::ProjectionMatrix:
            case ShaderConstantType::ModelToWorldMatrix:
            case ShaderConstantType::WorldToLightMatrix:
            case ShaderConstantType::LightProjectionMatrix:
            case ShaderConstantType::ShadowBiasMatrix:
            case ShaderConstantType::ShadowMatrix: {
                shader.setMatrix4Uniform(reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }

            case ShaderConstantType::DemoPartNormalizedTime: {
                shader.setFloatUniform(*reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }

            case ShaderConstantType::LightPositionInEyeSpace: {
                shader.setVec3Uniform(reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }

            default:
                break;
        }
    }
}

} // namespace Acidrain

