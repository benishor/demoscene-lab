#pragma once

#include <GLheaders.h>

class Texture {
public:

    Texture(int width, int height, unsigned char* buffer);
    virtual ~Texture();

    int getWidth() const;
    int getHeight() const;

    void use();

private:

    int width;
    int height;
    GLuint textureId = 0;
};