#include <Material.h>
#include <ShaderConstants.h>
#include <iostream>

namespace Acidrain {

    GLenum toGL(const BlendingConstant& c) {
        switch (c) {
            case BlendingConstant::SRC_COLOR:
                return GL_SRC_COLOR;
            case BlendingConstant::DST_COLOR:
                return GL_DST_COLOR;
            case BlendingConstant::ONE:
                return GL_ONE;
            case BlendingConstant::ZERO:
                return GL_ZERO;
            case BlendingConstant::SRC_ALPHA:
                return GL_SRC_ALPHA;
            case BlendingConstant::DST_ALPHA:
                return GL_DST_ALPHA;
            case BlendingConstant::ONE_MINUS_SRC_ALPHA:
                return GL_ONE_MINUS_SRC_ALPHA;
            case BlendingConstant::ONE_MINUS_DST_ALPHA:
                return GL_ONE_MINUS_DST_ALPHA;
            default:
                return GL_ZERO;
        }
    }

    void setMaterial(std::shared_ptr<Material>& material) {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        material->shader->use();

//        if (material->textures[TextureRole::Diffuse]) {
//            std::cout << "CAN has diffuse texture!!!!!!!!!11" << std::endl;
//            int textureUnit = 0;
//            glActiveTexture(GL_TEXTURE0 + textureUnit);
//            material->shader->setIntUniform(textureUnit, "diffuseMap");
//            material->textures[TextureRole::Diffuse]->use();
//        } else {
//            std::cout << "Cannot has diffuse texture!!!!!!!!!11" << std::endl;
//        }

        // int textureUnit = 0;
        // for (auto& kv : material->textures) {
        //     glActiveTexture(GL_TEXTURE0 + textureUnit);
        //     const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
        //     material->shader->setIntUniform(textureUnit, uniformName);
        //     kv.second->use();
        //     textureUnit++;
        // }

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

    void unsetMaterial(shared_ptr<Material>& material) {
        // int textureUnit = 0;
        // for (auto& kv : material->textures) {
        //     glActiveTexture(GL_TEXTURE0 + textureUnit);
        //     const char* uniformName = ShaderConstantNameResolver::nameFor(kv.first);
        //     material->shader->setIntUniform(textureUnit, uniformName);
        //     kv.second->unuse();
        //     textureUnit++;
        // }

        material->shader->unuse();

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