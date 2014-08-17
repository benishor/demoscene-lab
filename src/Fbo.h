#pragma once

#include <GLheaders.h>
#include <Texture.h>
#include <memory>

namespace Acidrain {

class Fbo {
public:

    explicit Fbo(int width, int height);
    virtual ~Fbo();

    void                        use();
    void                        unuse();

    int                         getWidth();
    int                         getHeight();

    GLuint                      getColorBufferId();
    GLuint                      getDepthBufferId();

    std::shared_ptr<Texture>    getTexture();

private:
    int     width;
    int     height;

    int     oldWidth;
    int     oldHeight;

    GLuint  frameBufferId;
    GLuint  colorBufferId;
    GLuint  depthBufferId;
};

} // namespace Acidrain