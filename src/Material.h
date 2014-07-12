#pragma once

#include <Colour.h>

enum class RenderType {
    Wireframe,
    Solid
};

struct Material {
    Colour ambient;
    Colour diffuse;
    Colour specular;
    RenderType renderType;
};
