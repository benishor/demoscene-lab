#include <Texture.h>

namespace Acidrain {

Texture::Texture(int w, int h, unsigned char* buffer)
    : width(w), height(h), destroyable(true) {

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // only set this when we do have mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // for versions up to 3
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 buffer);
}


Texture::Texture(GLuint textId, int w, int h)
    : textureId(textId), width(w), height(h), destroyable(false) {
}


Texture::~Texture() {
    if (destroyable)
        glDeleteTextures(1, &textureId);
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

void Texture::use() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Acidrain

