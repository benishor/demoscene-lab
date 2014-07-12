#pragma once

#include <Renderer.h>
#include <Randomizer.h>

class CubeScene {
public:
    CubeScene();

    void update(double secondsSinceLastUpdate);
    void render();

private:

    void setupCubePositionsAndRotations();

    Scene scene;
    Renderer renderer;
    Randomizer randomizer;
    double elapsedTimeInSeconds = 0;
};
