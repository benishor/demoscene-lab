#pragma once

struct Colour {
    union {
        struct {
            float r, g, b, a;
        };
        float array[4];
    };
};