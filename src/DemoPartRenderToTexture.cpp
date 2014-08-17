#include <DemoPartRenderToTexture.h>
#include <DemoData.h>

namespace Acidrain {

DemoPartRenderToTexture::DemoPartRenderToTexture(int s, RenderToTextureAction a)
    : slot(s), action(a) {
}

void DemoPartRenderToTexture::process(float normalizedTime) {
    if (action == RenderToTextureAction::Start)
        DemoData::fbos[slot]->use();
    else
        DemoData::fbos[slot]->unuse();
}


} // namespace Acidrain