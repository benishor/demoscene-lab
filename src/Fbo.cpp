#include <Fbo.h>
#include <DemoData.h>


#if defined (__APPLE__)
#include <OpenGL/gl3.h>
#endif

// redefinition but caused by OSX
// #define GL_TEXTURE_2D_MULTISAMPLE 0x9100

namespace Acidrain {

    Fbo::Fbo(int w, int h) : width(w), height(h) {
        glGenFramebuffers(1, &frameBufferId);
        glGenTextures(1, &colorBufferId);
        glGenRenderbuffers(1, &depthBufferId);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBufferId);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                4, // number of samples for multisampling
                                GL_RGBA8,
                                width, height,
                                false);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBufferId, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
    }

    Fbo::~Fbo() {
    }

    int Fbo::getWidth() {
        return width;
    }

    int Fbo::getHeight() {
        return height;
    }

    GLuint Fbo::getColorBufferId() {
        return colorBufferId;
    }

    GLuint Fbo::getDepthBufferId() {
        return depthBufferId;
    }

    void Fbo::use() {
        oldWidth = DemoData::WIDTH;
        oldHeight = DemoData::HEIGHT;

        DemoData::WIDTH = width;
        DemoData::HEIGHT = height;

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        glViewport(0, 0, width, height);
    }

    void Fbo::unuse() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, oldWidth, oldHeight);

        DemoData::WIDTH = oldWidth;
        DemoData::HEIGHT = oldHeight;
    }

    std::shared_ptr<Texture> Fbo::getTexture() {
        return std::shared_ptr<Texture>(new Texture(colorBufferId, width, height));
    }

} // namespace Acidrain