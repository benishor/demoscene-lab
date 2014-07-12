#pragma once

#include <Colour.h>

enum class RenderType {
    Wireframe,
    Solid
};

struct Material {
    Colour colour;
    RenderType renderType;
};
