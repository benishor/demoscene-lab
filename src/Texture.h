#pragma once

#include <GLheaders.h>

namespace Acidrain {

class Texture {
public:

    Texture(int width, int height, unsigned char* buffer);
    virtual ~Texture();

    int getWidth() const;
    int getHeight() const;

    void use();
    void unuse();

private:

    int width;
    int height;
    GLuint textureId = 0;
};

} // namespace Acidrain
