#pragma once

#include <memory>
#include <map>
#include <GLheaders.h>
#include <GLMheaders.h>
#include <Texture.h>
#include <Shader.h>

namespace Acidrain {

    using namespace std;
    using namespace glm;

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
        Emissive,
        Environment
    };

    typedef map<TextureRole, shared_ptr<Texture>> TextureMap;

    struct Material {
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        float shininess = 1.f;

        bool flatShaded = true;
        bool zBufferWrite = true;
        bool zBufferTest = true;
        bool cullFaces = false;
        bool cullFrontFaces = false;
        bool transparent = false;
        bool castsShadows = true;

        BlendingConstant blendSrcFactor = BlendingConstant::SRC_ALPHA;
        BlendingConstant blendDstFactor = BlendingConstant::ONE_MINUS_SRC_ALPHA;

        FillMode fillMode = FillMode::Solid;
        shared_ptr<Shader> shader;
        TextureMap textures;
    };

    void setMaterial(shared_ptr<Material>& material);
    void unsetMaterial(shared_ptr<Material>& material);

} // namespace Acidrain

