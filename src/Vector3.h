#pragma once

#include <math.h>
#include <iostream>

struct Vector3 {
    double x, y, z;

    void zero() {
        x = y = z = 0;
    }

    const Vector3& operator += (const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

inline Vector3 operator - (const Vector3& first, const Vector3& second) {
    return {
        first.x - second.x,
        first.y - second.y,
        first.z - second.z
    };
}

inline Vector3 operator + (const Vector3& first, const Vector3& second) {
    return {
        first.x + second.x,
        first.y + second.y,
        first.z + second.z
    };
}



inline Vector3 operator * (const Vector3& a, double amount) {
    return {
        a.x * amount,
        a.y * amount,
        a.z* amount
    };
}

inline Vector3 operator / (const Vector3& a, double amount) {
    return {
        a.x / amount,
        a.y / amount,
        a.z / amount
    };
}

inline Vector3 cross(const Vector3& first, const Vector3& second) {
    return {
        first.y* second.z - first.z * second.y,
        first.z* second.x - first.x * second.z,
        first.x* second.y - first.y * second.x,
    };
}

inline double length(const Vector3& a) {
    return sqrt(a.x * a.x +
                a.y * a.y +
                a.z * a.z);
}

inline Vector3 normalize(const Vector3& a) {
    double len = length(a);
    if (len > 0.0000001)
        return a / len;
    else
        return a;
}

