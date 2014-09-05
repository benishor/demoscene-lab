#include <Window.h>
#include <Timer.h>
#include <DemoEngine.h>
#include "Operator.h"

using namespace Acidrain;
using namespace std;

const float DEMO_LENGTH_IN_SECONDS = 10;

int main() {

    setupOctopussy();
    Window window(1024, 768, WindowType::Windowed);

    // --------------------------------------------------------------------------------------
    // Load resources
    // --------------------------------------------------------------------------------------

    DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    // DemoData::meshes.push_back(MeshGenerator::cube());
    DemoData::meshes.push_back(MeshGenerator::grid(30, 30));

    auto material = shared_ptr<Material>(new Material());
    DemoData::materials.push_back(material);


    // --------------------------------------------------------------------------------------
    // Set up demo parts
    // --------------------------------------------------------------------------------------

    DemoPartClear demoPartClear;
    demoPartClear.startTime = 0;
    demoPartClear.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartClear.color = glm::vec4(0.1, 0, 0.2, 1);

    DemoPartScene demoPartScene;
    demoPartScene.startTime = 0;
    demoPartScene.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartScene.cameraName = "cam1";
    demoPartScene.lightName = "light1";

    Scene* scene = new Scene();
    demoPartScene.scene = shared_ptr<Scene>(scene);

    auto meshNode = shared_ptr<MeshNode>(new MeshNode());
    meshNode->mesh      = DemoData::meshes[1];
    meshNode->material  = DemoData::materials[0];
    meshNode->position  = glm::vec3(0, 0, 0);
    meshNode->scale     = glm::vec3(0.5);
    // meshNode->rotation  = glm::angleAxis(2.25f , glm::vec3(1.0f, 0.0f, 0.0f));

    auto planeNode = shared_ptr<MeshNode>(new MeshNode());
    planeNode->mesh      = DemoData::meshes[2];
    planeNode->material  = DemoData::materials[0];
    planeNode->position  = glm::vec3(0, 0, 0);
    planeNode->scale     = glm::vec3(4);
    planeNode->rotation  = glm::angleAxis(3.141529f / 2.0f , glm::vec3(1.0f, 0.0f, 0.0f));

    // auto lightMeshNode = shared_ptr<MeshNode>(new MeshNode());
    // lightMeshNode->mesh      = DemoData::meshes[0];
    // lightMeshNode->material  = DemoData::materials[0];
    // lightMeshNode->position  = glm::vec3(1, 2, 1);
    // lightMeshNode->scale     = glm::vec3(0.1);

    auto camNode = shared_ptr<CameraNode>(new CameraNode());
    camNode->name 		= "cam1";
    camNode->position 	= glm::vec3(0, 1, 2);
    camNode->target 	= glm::vec3(0, -0.8, 0);
    camNode->fov 		= 45;

    auto lightNode = shared_ptr<LightNode>(new LightNode());
    lightNode->name 	 = "light1";
    lightNode->position  = glm::vec3(-5, 5, 0);
    lightNode->lightType = LightType::Point;

    scene->tree->add(planeNode);
    scene->tree->add(meshNode);
    // scene->tree->add(lightMeshNode);

    scene->tree->add(camNode);
    scene->tree->add(lightNode);

    // --------------------------------------------------------------------------------------
    // Run demo
    // --------------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < DEMO_LENGTH_IN_SECONDS) {

        double elapsedSeconds = timer.secondsSinceStart();

        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene.process(demoPartScene.normalizeTime(elapsedSeconds));

        window.present();
    }

    return 0;
}