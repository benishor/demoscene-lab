#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <Material.h>

using namespace std;
using namespace glm;

struct Particle {
    vec3 position;
    vec3 velocity;
    float maxAge;
    float age;
    float speed;
    float startSize;
    float endSize;
    vec4 startColor;
    vec4 endColor;
};

struct ParticleVertex {
    vec3   position;
    vec4   color;
    vec2   uv;
};

class ParticleSystem {
public:
    ParticleSystem();

    void update(double elapsedSeconds);
    void render();

    Particle* newParticle();

    Material material;

private:
    vector<Particle*> particles;
    vector<Particle*> deadParticles;
    vector<ParticleVertex> vertexBuffer;

    GLuint vboId = 0; // not initialized
    float* vboData = nullptr;
};

class ParticleEmitter {
public:

    ParticleEmitter(ParticleSystem& system) : particleSystem(system) {}

    void update(double elapsedSeconds);

    double elapsedTime = 0;
    vec3 position = vec3(0, 0 , 0);
    ParticleSystem& particleSystem;
};
