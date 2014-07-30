#pragma once

#include <DemoPart.h>
#include <GLMheaders.h>

namespace Acidrain {

struct DemoPartClear : DemoPart {

	glm::vec4 	color = glm::vec4(0, 0, 0, 0);

	bool		clearDepth = true;
	bool		clearColor = true;
	bool		clearStencil = true;

    void process(float normalizedTime) override;
};


} // namespace Acidrain