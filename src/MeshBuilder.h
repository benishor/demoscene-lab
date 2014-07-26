#pragma once
#include <Mesh.h>

class MeshBuilder {
public:

    static Mesh cube();
    static Mesh sphere(int longitudePoints, int latitudePoints);
    static Mesh grid(int xSegments, int ySegments);
};
