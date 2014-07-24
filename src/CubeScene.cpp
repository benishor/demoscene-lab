#include <CubeScene.h>
#include <MeshBuilder.h>
#include <GLheaders.h>
#include <TextureGenerator.h>
#include <ShaderManager.h>

using namespace glm;

static vec3 rotationAxis[3];

CubeScene::CubeScene() {
    // for (int i = 0; i < 2; i++)
    //     scene.meshes.push_back(MeshBuilder::cube());

    camera.position = vec3(0, 0, 2.5);
    camera.target = vec3(0, 0, 0);

    ShaderManager::init();

    scene.meshes.push_back(MeshBuilder::cube());
    scene.meshes.push_back(MeshBuilder::sphere(50, 50));
    scene.meshes.push_back(MeshBuilder::cube());


    scene.meshes[0].scale = vec3(2);
    scene.meshes[0].material.ambient = vec4(0.2, 0.2, 0.2, 1);
    scene.meshes[0].material.diffuse = vec4(1, 0, 0, 1);
    scene.meshes[0].material.specular = vec4(1, 1, 1, 1);
    scene.meshes[0].material.shininess = 2;
    scene.meshes[0].material.fillMode = FillMode::Solid;
    scene.meshes[0].material.shader = ShaderManager::get(0);
    scene.meshes[0].material.textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 8, vec4(1, 1, 1, 0.9), vec4(0, 0.3, 0.1, 0.9))
        .roll(0, 4, 4)
        .getTexture(0)
    );

    scene.meshes[2].scale = vec3(2);
    scene.meshes[2].material.ambient = vec4(0.2, 0.2, 0.2, 1);
    scene.meshes[2].material.diffuse = vec4(0.5, 0.4, 0.8, 1);
    scene.meshes[2].material.specular = vec4(1, 0, 0, 1);
    scene.meshes[2].material.fillMode = FillMode::Solid;
    scene.meshes[2].material.shader = ShaderManager::get(0);
    scene.meshes[2].material.isOpaque = false;
    scene.meshes[2].material.shouldWriteToDepthBuffer = true;
    scene.meshes[2].material.shouldTestDepthBuffer = true;
    scene.meshes[2].material.blendingSourceConstant = BlendingConstant::SRC_COLOR;
    scene.meshes[2].material.blendingDestinationConstant = BlendingConstant::ONE;
    scene.meshes[2].material.textures.push_back(
        TextureGenerator(256, 256)
        .envMap(0, 100)
        .getTexture(0)
    );

    scene.meshes[1].material.ambient = vec4(0.2, 0.2, 0.2, 1);
    scene.meshes[1].material.diffuse = vec4(0.6, 0.4, 0.6, 1);
    scene.meshes[1].material.specular = vec4(1, 1, 1, 1);
    scene.meshes[1].material.fillMode = FillMode::Solid;
    //)scene.meshes[1].material.textures.push_back(TextureGenerator(256, 256).checkerBoard(0, 32, vec4(1, 1, 1, 0.9}, )ec4(0, 0.3, 0.1, 0.9}).)oll(0, 0, 32).getTexture(0));
    scene.meshes[1].material.textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 16, vec4(1, 1, 1, 0.9), vec4(0, 0.3, 0.1, 0.9))
        .roll(0, 8, 8)
        .getTexture(0)
    );
    // scene.meshes[1].material.textures.push_back(
    //     TextureGenerator(256, 256)
    //     .envMap(0, 10)
    //     .getTexture(0)
    // );
    scene.meshes[1].material.shader = ShaderManager::get(0);
    // scene.meshes[1].material.isOpaque = false;
    // scene.meshes[1].material.shouldWriteToDepthBuffer = false;
    // scene.meshes[1].material.shouldTestDepthBuffer = false;
    // scene.meshes[1].material.blendingSourceConstant = BlendingConstant::ONE;
    // scene.meshes[1].material.blendingDestinationConstant = BlendingConstant::ONE;


    setupCubePositionsAndRotations();

    // Create a point light
    PointLight light;
    light.position = vec3(0, 0, 4);
    light.ambient = vec4(0.2, 0.2, 0.2, 1);
    light.diffuse = vec4(0.8, 0.8, 0.8, 1);
    light.specular = vec4(1, 1, 1, 1);

    scene.pointLights.push_back(light);
}

float lightIntensityForCurrentTime(double elapsedTimeInSeconds) {
    // return fabs(sin((elapsedTimeInSeconds / 5.0) * 3.1415926 * 2));
    return 1;
}

void CubeScene::update(double secondsSinceLastUpdate) {
    int i = 0;
    for (auto& mesh : scene.meshes) {
        mesh.rotation *= angleAxis(static_cast<float>(1 * secondsSinceLastUpdate), rotationAxis[i]);
        mesh.computeModelToWorldMatrix();
        i++;
    }

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
        mesh.position = vec3(static_cast<float>(i * 2.5 - 2.5), 0, 0);
        rotationAxis[i] = vec3(
                                randomizer.randomUnitDouble(),
                                randomizer.randomUnitDouble(),
                                randomizer.randomUnitDouble()
                            );
        i++;
    }
}
