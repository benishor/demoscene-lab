#pragma once

#include <cstdint>
#include <GLMheaders.h>

typedef glm::vec4 Colour;

inline uint32_t asInt(const glm::vec4& col) {
    unsigned char rr = static_cast<unsigned char>(col.r * 255);
    unsigned char gg = static_cast<unsigned char>(col.g * 255);
    unsigned char bb = static_cast<unsigned char>(col.b * 255);
    unsigned char aa = static_cast<unsigned char>(col.a * 255);
    return rr + (gg << 8) + (bb << 16) + (aa << 24);
}
