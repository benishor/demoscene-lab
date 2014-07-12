#pragma once

#include <ostream>
#include <Mesh.h>

inline std::ostream& operator << (std::ostream& out, const Vector3& a) {
    out << "{x: " << a.x << ", y: " << a.y << ", z: " << a.z << "}";
    return out;
}


void drawNormals(const Mesh& mesh);