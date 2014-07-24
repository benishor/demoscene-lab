#pragma once

#include <Colour.h>
#include <glm/vec3.hpp>

struct PointLight {
    glm::vec3 position;
    float intensity = 1.0;
    Colour ambient;
    Colour diffuse;
    Colour specular;
};