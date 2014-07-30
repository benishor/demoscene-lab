#pragma once

#include <memory>
#include <map>
#include <GLheaders.h>
#include <GLMheaders.h>
#include <Texture.h>
#include <Shader.h>

namespace Acidrain {

enum class FillMode {
    Wireframe,
    Solid
};

enum class BlendingConstant {
    SRC_COLOR,
    DST_COLOR,
    ONE,
    ZERO,
    SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_ALPHA,
};

enum class TextureRole {
    Diffuse,
    Specular,
    Normal,
    Bump
};

typedef std::map<TextureRole, std::shared_ptr<Texture>> TextureMap;

struct Material {
    glm::vec4   		ambient;
    glm::vec4   		diffuse;
    glm::vec4   		specular;
    int         		shininess;

    bool flatShaded     = true;
    bool zBufferWrite   = true;
    bool zBufferTest    = true;
    bool cullFaces      = false;
    bool cullFrontFaces = false;
    bool transparent    = false;

    BlendingConstant blendSrcFactor = BlendingConstant::SRC_ALPHA;
    BlendingConstant blendDstFactor = BlendingConstant::ONE_MINUS_SRC_ALPHA;

    FillMode 				fillMode = FillMode::Solid;
    std::shared_ptr<Shader> shader;
    TextureMap				textures;
};

void setMaterial(std::shared_ptr<Material>& material);

} // namespace Acidrain

