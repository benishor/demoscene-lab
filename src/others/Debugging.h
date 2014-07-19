#pragma once

#include <ostream>
#include <Mesh.h>
#include <iomanip>
#include <Texture.h>

inline std::ostream& operator << (std::ostream& out, const Vector3& a) {
    out << std::setprecision(5) << std::fixed << "{x: " << a.x << ", y: " << a.y << ", z: " << a.z << "}";
    return out;
}

inline std::ostream& operator << (std::ostream& out, const UV& a) {
    out << std::setprecision(5) << std::fixed << "(u: " << a.u << ", v: " << a.v << ")";
    return out;
}


void drawNormals(const Mesh& mesh);
