#pragma once

#include <GLheaders.h>

namespace Acidrain {

class Texture {
public:

    Texture(int width, int height, unsigned char* buffer);
    Texture(GLuint textureId, int width, int height);

    virtual ~Texture();

    int 	getWidth() const;
    int 	getHeight() const;

    void 	use();
    void 	unuse();

private:

    int 	width;
    int 	height;
    bool 	destroyable;
    GLuint 	textureId = 0;
};

} // namespace Acidrain
