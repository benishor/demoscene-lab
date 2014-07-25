#include <Particle.h>
#include <algorithm>
#include <Randomizer.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include <TextureGenerator.h>
#include <ShaderManager.h>

ParticleSystem::ParticleSystem() {
    material.ambient = vec4(0.2, 0.2, 0.2, 1);
    material.diffuse = vec4(0.5, 0.4, 0.8, 1);
    material.specular = vec4(1, 0, 0, 1);
    material.fillMode = FillMode::Solid;
    // material.shader = ShaderManager::get(0);
    material.isOpaque = false;
    material.shouldWriteToDepthBuffer = false;
    material.shouldTestDepthBuffer = true;
    material.blendingSourceConstant = BlendingConstant::SRC_ALPHA;
    material.blendingDestinationConstant = BlendingConstant::ONE;
    material.textures.push_back(
        TextureGenerator(64, 64)
        .lens(0, 32)
        .getTexture(0)
    );

    vertexBuffer.reserve(3000);
    particles.reserve(3000);
}

void ParticleSystem::update(double elapsedSeconds) {
    float elapsed = static_cast<float>(elapsedSeconds);
    auto it = std::begin(particles);

    while (it != std::end(particles)) {
        auto particle = *it;
        particle->age += elapsed;
        if (particle->age > particle->maxAge) {
            deadParticles.push_back(particle);
            it = particles.erase(it);
        } else {
            particle->position += particle->velocity * particle->speed * elapsed;
            ++it;
        }
    }

    // build buffer
    vertexBuffer.resize(particles.size() * 4);

    int i = 0;
    for (auto particle : particles) {

        float normalizedAge = particle->age / particle->maxAge;
        float size = lerp<float>(particle->startSize, particle->endSize, normalizedAge);
        vec4 color = lerp(particle->startColor, particle->endColor, normalizedAge);

        ParticleVertex& v1 = vertexBuffer[i++];
        ParticleVertex& v2 = vertexBuffer[i++];
        ParticleVertex& v3 = vertexBuffer[i++];
        ParticleVertex& v4 = vertexBuffer[i++];

        v1.position = vec3(particle->position.x - size / 2, particle->position.y - size / 2, particle->position.z);
        v1.uv = vec2(0.0, 0.0);
        v1.color = color;

        v2.position = vec3(particle->position.x + size / 2, particle->position.y - size / 2, particle->position.z);
        v2.uv = vec2(1.0, 0.0);
        v2.color = color;

        v3.position = vec3(particle->position.x + size / 2, particle->position.y + size / 2, particle->position.z);
        v3.uv = vec2(1.0, 1.0);
        v3.color = color;

        v4.position = vec3(particle->position.x - size / 2, particle->position.y + size / 2, particle->position.z);
        v4.uv = vec2(0.0, 1.0);
        v4.color = color;
    }
}

constexpr char* BUFFER_OFFSET(int i) {
    return (char*)NULL + i * sizeof(float);
}

void ParticleSystem::render() {

    if (vboId == 0) {
        glGenBuffers(1, &vboId);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertex) * vertexBuffer.size() , &(vertexBuffer[0]), GL_DYNAMIC_DRAW);

    material.shader = ShaderManager::get(1);
    material.use();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(ParticleVertex), BUFFER_OFFSET(0));
    glColorPointer(4, GL_FLOAT, sizeof(ParticleVertex), BUFFER_OFFSET(3));
    glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVertex), BUFFER_OFFSET(7));

    glDrawArrays(GL_QUADS, 0, vertexBuffer.size());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    material.unuse();

}

Particle* ParticleSystem::newParticle() {
    Particle* result;

    if (deadParticles.empty()) {
        result = new Particle();
        particles.push_back(result);
    } else {
        result = deadParticles.back();
        deadParticles.pop_back();
    }

    result->age = 0;

    return result;
}

static Randomizer randomizer;

void ParticleEmitter::update(double elapsedSeconds) {
    elapsedTime += elapsedSeconds;
    double spawnIntervalInSeconds = 0.1;
    while (elapsedTime > spawnIntervalInSeconds) {
        int particlesToSpawn = 20;
        for (int i = 0; i < particlesToSpawn; i++) {
            Particle* p = particleSystem.newParticle();
            p->age = 0;
            p->maxAge = randomizer.randomUnitDouble() * 4 + 0.3;
            p->position = position;
            p->velocity = normalize(vec3(
                                        randomizer.randomUnitDouble() - 0.5,
                                        randomizer.randomUnitDouble() - 0.5,
                                        randomizer.randomUnitDouble() - 0.5
                                    ));
            p->speed = randomizer.randomUnitDouble() * 3 + 1;
            p->startSize = 1;
            p->endSize = 4;
            p->endColor = vec4(1, 1, 1, 0.6);

            // vec3 col = vec3(randomizer.randomUnitDouble(), randomizer.randomUnitDouble(), randomizer.randomUnitDouble());
            vec3 col = randomizer.randomUnitDouble() < 0.5 ? vec3(0.7, 0.2, 0.4) : vec3(0, 0, 1);
            p->startColor = vec4(col, 0.01);
        }
        elapsedTime -= spawnIntervalInSeconds;
    }
}