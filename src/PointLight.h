#pragma once

#include <Vector3.h>
#include <Colour.h>

struct PointLight {
    Vector3 position;
    float intensity = 1.0;
    Colour ambient;
    Colour diffuse;
    Colour specular;
};