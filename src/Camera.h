#pragma once

#include <glm/vec3.hpp>

struct Camera {

    glm::vec3 position = {0, 0, 1};
    glm::vec3 target = {0, 0, 0};
    int rollAngleInDegrees = 0;

    void set();
};