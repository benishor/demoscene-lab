#include <CubeScene.h>
#include <MeshBuilder.h>
#include <GLheaders.h>

CubeScene::CubeScene() {
    // for (int i = 0; i < 2; i++)
    //     scene.meshes.push_back(MeshBuilder::cube());

    scene.meshes.push_back(MeshBuilder::cube());
    scene.meshes.push_back(MeshBuilder::sphere(30, 30));
    scene.meshes.push_back(MeshBuilder::cube());

    scene.meshes[0].material.ambient = {0.2, 0.2, 0.2, 1};
    scene.meshes[0].material.diffuse = {1, 0, 0, 1};
    scene.meshes[0].material.specular = {1, 1, 1, 1};
    scene.meshes[0].material.renderType = RenderType::Solid;

    scene.meshes[2].material.ambient = {0.8, 0.8, 0.8, 1};
    scene.meshes[2].material.diffuse = {1, 1, 1, 1};
    scene.meshes[2].material.specular = {1, 1, 1, 1};
    scene.meshes[2].material.renderType = RenderType::Solid;

    scene.meshes[1].material.ambient = {0.2, 0.2, 0.2, 1};
    scene.meshes[1].material.diffuse = {0, 0, 1, 1};
    scene.meshes[1].material.specular = {1, 1, 1, 1};
    scene.meshes[1].material.renderType = RenderType::Solid;

    setupCubePositionsAndRotations();

    // Create a point light
    PointLight light;
    light.position = {0, 0, 4};
    light.ambient = {0.2, 0.2, 0.2, 1};
    light.diffuse = {0.5, 0.5, 0.5, 1};
    light.specular = {1, 1, 1, 1};

    scene.pointLights.push_back(light);
}

float lightIntensityForCurrentTime(double elapsedTimeInSeconds) {
    return fabs(sin((elapsedTimeInSeconds / 5.0) * 3.1415926 * 2));
}

void CubeScene::update(double secondsSinceLastUpdate) {
    for (auto& mesh : scene.meshes)
        mesh.rotationAngle += 120 * secondsSinceLastUpdate;

    scene.pointLights[0].intensity = lightIntensityForCurrentTime(elapsedTimeInSeconds);
    elapsedTimeInSeconds += secondsSinceLastUpdate;
}

void CubeScene::render() {
    // Clear screen
    glClearColor(0.05f, 0.0f, 0.1f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move the camera backwards so we can see more
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0f);

    renderer.render(scene);
}

void CubeScene::setupCubePositionsAndRotations() {
    int i = 0;
    for (auto& mesh : scene.meshes) {
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
