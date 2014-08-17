#pragma once

#include <DemoPart.h>

namespace Acidrain {

enum class RenderToTextureAction {
	Start,
	Stop
};

struct DemoPartRenderToTexture : DemoPart {

	DemoPartRenderToTexture(int slot, RenderToTextureAction action);

    void process(float normalizedTime) override;

    int slot;
    RenderToTextureAction action;
};


} // namespace Acidrain