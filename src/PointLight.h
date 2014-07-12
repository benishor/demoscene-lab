#pragma once

#include <Vector3.h>
#include <Colour.h>

struct PointLight {
    Vector3 position;
    Colour ambient;
    Colour diffuse;
    Colour specular;
    Colour emissive;
};