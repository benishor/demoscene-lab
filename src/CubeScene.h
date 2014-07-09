#pragma once

#include <Mesh.h>
#include <Randomizer.h>

class CubeScene {
public:
    CubeScene();

    void update(double secondsSinceLastUpdate);
    void render();

private:

    void setupCubePositionsAndRotations();

    Mesh meshes[3];
    Randomizer randomizer;
};
