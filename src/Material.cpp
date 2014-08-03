#include <Material.h>
#include <ShaderConstants.h>

namespace Acidrain {

GLenum toGL(const BlendingConstant& c) {
    switch (c) {
        case BlendingConstant::SRC_COLOR: return GL_SRC_COLOR;
        case BlendingConstant::DST_COLOR: return GL_DST_COLOR;
        case BlendingConstant::ONE: return GL_ONE;
        case BlendingConstant::ZERO: return GL_ZERO;
        case BlendingConstant::SRC_ALPHA: return GL_SRC_ALPHA;
        case BlendingConstant::DST_ALPHA: return GL_DST_ALPHA;
        case BlendingConstant::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendingConstant::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
        default: return GL_ZERO;
    }
}

void setMaterial(std::shared_ptr<Material>& material) {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    material->shader->use();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material->ambient[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material->diffuse[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material->specular[0]);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);

    int textureUnit = 0;
    for (auto& kv : material->textures) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
        material->shader->setIntUniform(textureUnit, uniformName);
        kv.second->use();
        textureUnit++;
    }

    if (material->transparent) {
        glEnable(GL_BLEND);
        glBlendFunc(toGL(material->blendSrcFactor), toGL(material->blendDstFactor));
    } else {
        glDisable(GL_BLEND);
    }

    if (material->zBufferTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (material->zBufferWrite)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    if (material->cullFaces) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(material->cullFrontFaces ? GL_FRONT : GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void unsetMaterial(std::shared_ptr<Material>& material) {
    material->shader->unuse();

    int textureUnit = 0;
    for (auto& kv : material->textures) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
        material->shader->setIntUniform(textureUnit, uniformName);
        kv.second->unuse();
        textureUnit++;
    }

    if (material->transparent)
        glDisable(GL_BLEND);

    if (!material->zBufferTest)
        glEnable(GL_DEPTH_TEST);

    if (!material->zBufferWrite)
        glDepthMask(GL_TRUE);

    if (material->cullFaces)
        glDisable(GL_CULL_FACE);
}

} // namespace Acidrain