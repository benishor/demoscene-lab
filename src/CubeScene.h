#pragma once

#include <Renderer.h>
#include <Randomizer.h>
#include <Camera.h>
#include <Particle.h>

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
    ParticleSystem particleSystem;
    ParticleEmitter particleEmitter = ParticleEmitter(particleSystem);
    double elapsedTimeInSeconds = 0;
};
