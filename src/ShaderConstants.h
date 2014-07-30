#pragma once

#include <Shader.h>
#include <Texture.h>
#include <map>
#include <Material.h>

namespace Acidrain {

enum class ShaderConstantType {
    ViewMatrix,
    ProjectionMatrix,
    ModelToWorldMatrix,
    DemoPartNormalizedTime
};

class ShaderConstantNameResolver {
public:

    static const char* nameFor(const ShaderConstantType& type);
    static const char* nameFor(const TextureRole& type);
};

struct ShaderConstants {
    std::map<ShaderConstantType, void*> constants;

    void add(ShaderConstantType type, void* value);
    void set(Shader& shader);
};

} // namespace Acidrain