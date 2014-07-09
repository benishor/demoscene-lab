#include <CubeScene.h>
#include <MeshBuilder.h>
#include <GLheaders.h>

CubeScene::CubeScene() {
    for (auto& mesh : meshes)
        mesh = MeshBuilder::cube();

    setupCubePositionsAndRotations();
}

void CubeScene::update(double secondsSinceLastUpdate) {
    for (auto& mesh : meshes)
        mesh.rotationAngle += 30 * secondsSinceLastUpdate;
}

void CubeScene::render() {
    // Clear screen
    glClearColor(0.05f, 0.0f, 0.1f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move the camera backwards so we can see more
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0f);

    // Draw cubes
    for (auto& mesh : meshes) {
        glPushMatrix();
        mesh.applyTransformation();
        mesh.renderWireframe();
        glPopMatrix();
    }
}

void CubeScene::setupCubePositionsAndRotations() {
    int i = 0;
    for (auto& mesh : meshes) {
        mesh.position = {i * 1.5 - 1.5, 0, 0};
        mesh.rotationAxis = {
            randomizer.randomUnitDouble(),
            randomizer.randomUnitDouble(),
            randomizer.randomUnitDouble()
        };
        mesh.rotationAngle = randomizer.randomUnitDouble() * 360;

        i++;
    }
}
