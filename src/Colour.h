#pragma once

struct Colour {
    union {
        struct {
            float r, g, b, a;
        };
        float array[4];
    };
};

inline Colour operator * (const Colour& col, float amount) {
	return {
		col.r * amount,
		col.g * amount,
		col.b * amount,
		col.a
	};
}