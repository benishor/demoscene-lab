#include <Material.h>
#include <GLheaders.h>

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

void Material::use() const {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.array);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    if (isFlatShaded)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);

    if (isOpaque) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(toGL(blendingSourceConstant), toGL(blendingDestinationConstant));
    }

    if (shouldTestDepthBuffer)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (shouldWriteToDepthBuffer)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    if (shouldCullFaces) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(shouldCullFrontFaces ? GL_FRONT : GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    if (texture) {
        glEnable(GL_TEXTURE_2D);
        texture->use();
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    if (shader) {
        shader->use();
    }
}

void Material::unuse() const {
    if (texture)
        texture->unuse();

    if (shader)
        shader->unuse();
}