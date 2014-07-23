#include <CubeScene.h>
#include <MeshBuilder.h>
#include <GLheaders.h>
#include <TextureGenerator.h>
#include <ShaderManager.h>

CubeScene::CubeScene() {
    // for (int i = 0; i < 2; i++)
    //     scene.meshes.push_back(MeshBuilder::cube());

    camera.position = glm::vec3(0, 3, 3);
    camera.target = {0, 0, 0};

    ShaderManager::init();

    scene.meshes.push_back(MeshBuilder::cube());
    scene.meshes.push_back(MeshBuilder::sphere(50, 50));
    scene.meshes.push_back(MeshBuilder::cube());

    scene.meshes[0].material.ambient = {0.2, 0.2, 0.2, 1};
    scene.meshes[0].material.diffuse = {1, 0, 0, 1};
    scene.meshes[0].material.specular = {1, 1, 1, 1};
    scene.meshes[0].material.fillMode = FillMode::Solid;
    scene.meshes[0].material.shader = ShaderManager::get(2);
    scene.meshes[0].material.textures.push_back(TextureGenerator(256, 256).checkerBoard(0, 32, {1, 1, 1, 0.9}, {0, 0.3, 0.1, 0.9}).roll(0, 0, 32).getTexture(0));

    scene.meshes[2].material.ambient = {0.2, 0.2, 0.2, 1};
    scene.meshes[2].material.diffuse = {0.5, 0.4, 0.8, 1};
    scene.meshes[2].material.specular = {1, 1, 1, 1};
    scene.meshes[2].material.fillMode = FillMode::Solid;
    scene.meshes[2].material.shader = ShaderManager::get(1);
    scene.meshes[2].material.isOpaque = true;
    scene.meshes[2].material.shouldWriteToDepthBuffer = true;
    scene.meshes[2].material.shouldTestDepthBuffer = true;
    scene.meshes[2].material.blendingSourceConstant = BlendingConstant::ONE;
    scene.meshes[2].material.blendingDestinationConstant = BlendingConstant::ONE;
    scene.meshes[2].material.textures.push_back(TextureGenerator(256, 256).checkerBoard(0, 32, {1, 1, 1, 0.9}, {0, 0.3, 0.1, 0.9}).roll(0, 0, 32).getTexture(0));

    scene.meshes[1].material.ambient = {0.2, 0.2, 0.2, 1};
    scene.meshes[1].material.diffuse = {0.6, 0.4, 0.6, 1};
    scene.meshes[1].material.specular = {1, 1, 1, 1};
    scene.meshes[1].material.fillMode = FillMode::Solid;
    // scene.meshes[1].material.textures.push_back(TextureGenerator(256, 256).checkerBoard(0, 32, {1, 1, 1, 0.9}, {0, 0.3, 0.1, 0.9}).roll(0, 0, 32).getTexture(0));
    scene.meshes[1].material.textures.push_back(TextureGenerator(256, 256).checkerBoard(0, 32, {1, 1, 1, 0.9}, {0, 0.3, 0.1, 0.9}).roll(0, 0, 0).getTexture(0));
    scene.meshes[1].material.textures.push_back(TextureGenerator(256, 256).envMap(0, 10).getTexture(0));
    scene.meshes[1].material.shader = ShaderManager::get(2);
    // scene.meshes[1].material.isOpaque = false;
    // scene.meshes[1].material.shouldWriteToDepthBuffer = false;
    // scene.meshes[1].material.shouldTestDepthBuffer = false;
    // scene.meshes[1].material.blendingSourceConstant = BlendingConstant::ONE;
    // scene.meshes[1].material.blendingDestinationConstant = BlendingConstant::ONE;


    setupCubePositionsAndRotations();

    // Create a point light
    PointLight light;
    light.position = {0, 0, 4};
    light.ambient = {0.2, 0.2, 0.2, 1};
    light.diffuse = {0.8, 0.8, 0.8, 1};
    light.specular = {1, 1, 1, 1};

    scene.pointLights.push_back(light);
}

float lightIntensityForCurrentTime(double elapsedTimeInSeconds) {
    // return fabs(sin((elapsedTimeInSeconds / 5.0) * 3.1415926 * 2));
    return 1;
}

void CubeScene::update(double secondsSinceLastUpdate) {
    for (auto& mesh : scene.meshes)
        mesh.rotationAngle += 30 * secondsSinceLastUpdate;

    scene.pointLights[0].intensity = lightIntensityForCurrentTime(elapsedTimeInSeconds);
    elapsedTimeInSeconds += secondsSinceLastUpdate;
}

void CubeScene::render() {
    // Clear screen
    glClearColor(0.05f, 0.0f, 0.1f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render(scene, camera);
}

void CubeScene::setupCubePositionsAndRotations() {
    int i = 0;
    for (auto& mesh : scene.meshes) {
        mesh.position = {static_cast<double>(i * 3 - 3), 0, 0};
        mesh.rotationAxis = {
            randomizer.randomUnitDouble(),
            randomizer.randomUnitDouble(),
            randomizer.randomUnitDouble()
            // 1, 1, 0
        };
        mesh.rotationAngle = randomizer.randomUnitDouble() * 360;

        i++;
    }
}
