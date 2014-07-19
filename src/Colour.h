#pragma once

#include <cstdint>

struct Colour {
    union {
        struct {
            float r, g, b, a;
        };
        float array[4];
    };

    uint32_t asInt() {
        unsigned char rr = static_cast<unsigned char>(r * 255);
        unsigned char gg = static_cast<unsigned char>(g * 255);
        unsigned char bb = static_cast<unsigned char>(b * 255);
        unsigned char aa = static_cast<unsigned char>(a * 255);
        return rr + (gg << 8) + (bb << 16) + (aa << 24);
    }

};

inline Colour operator * (const Colour& col, float amount) {
    return {
        col.r * amount,
        col.g * amount,
        col.b * amount,
        col.a
    };
}