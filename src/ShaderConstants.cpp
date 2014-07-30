#include <ShaderConstants.h>

namespace Acidrain {

const char* ShaderConstantNameResolver::nameFor(const ShaderConstantType& type) {
    switch (type) {
        case ShaderConstantType::ViewMatrix: return "vmtx";
        case ShaderConstantType::ProjectionMatrix: return "pmtx";
        case ShaderConstantType::ModelToWorldMatrix: return "mmtx";
        case ShaderConstantType::DemoPartNormalizedTime: return "dptimen";
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
        switch (kv.first) {
            case ShaderConstantType::ViewMatrix:
            case ShaderConstantType::ProjectionMatrix:
            case ShaderConstantType::ModelToWorldMatrix: {
                const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
                shader.setMatrixUniform(reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }
            case ShaderConstantType::DemoPartNormalizedTime: {
                const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
                shader.setFloatUniform(*reinterpret_cast<float*>(kv.second), uniformName);
                break;
            }
            default:
                break;
        }
    }
}

} // namespace Acidrain

