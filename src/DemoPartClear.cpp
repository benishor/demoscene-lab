#include <DemoPartClear.h>
#include <GLheaders.h>

namespace Acidrain {

void DemoPartClear::process(float normalizedTime) {

    glClearColor(color.r, color.g, color.b, color.a);

    GLbitfield mask = 0;
    if (clearDepth)
        mask |= GL_DEPTH_BUFFER_BIT;

    if (clearColor)
        mask |= GL_COLOR_BUFFER_BIT;

    if (clearStencil)
        mask |= GL_STENCIL_BUFFER_BIT;

    glClear(mask);
}

} // namespace Acidrain
