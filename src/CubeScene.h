#pragma once

#include <Renderer.h>
#include <Randomizer.h>
#include <Camera.h>

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
    Camera camera;
    double elapsedTimeInSeconds = 0;
};
