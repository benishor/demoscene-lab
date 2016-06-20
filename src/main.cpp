#include <Window.h>
#include <Timer.h>
#include <DemoEngine.h>

using namespace Acidrain;
using namespace std;

const float DEMO_LENGTH_IN_SECONDS = 10;

int main() {

    Window window(1920, 1080, WindowType::Fullscreen);

    // --------------------------------------------------------------------------------------
    // Load resources
    // --------------------------------------------------------------------------------------

    DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    DemoData::meshes.push_back(MeshGenerator::grid(30, 30));
    DemoData::meshes.push_back(MeshGenerator::cube());

    auto material = shared_ptr<Material>(new Material());
    material->ambient = glm::vec4(0.1, 0.1, 0.1, 1);
    material->diffuse = glm::vec4(1, 0.2, 0.7, 1);
    material->specular = glm::vec4(1, 1, 1, 1);
    DemoData::materials.push_back(material);

    material = make_shared<Material>();
    material->ambient = glm::vec4(0.1, 0.1, 0.1, 1);
    material->diffuse = glm::vec4(0.2, 1.0, 0.7, 1);
    material->specular = glm::vec4(1, 1, 1, 1);
    material->castsShadows = false;
    DemoData::materials.push_back(material);

    material = make_shared<Material>();
    material->ambient = glm::vec4(0.1, 0.1, 0.1, 1);
    material->diffuse = glm::vec4(1.0, 0.7, 0.2, 1);
    material->specular = glm::vec4(1, 1, 1, 1);
    material->castsShadows = true;
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
    planeNode->material  = DemoData::materials[1];
    planeNode->position  = glm::vec3(0, 0, 0);
    planeNode->scale     = glm::vec3(10);
    planeNode->rotation  = glm::angleAxis(3.141529f / 2.0f , glm::vec3(1.0f, 0.0f, 0.0f));

    auto cubeNode = make_shared<MeshNode>();
    cubeNode->mesh      = DemoData::meshes[0];
    cubeNode->material  = DemoData::materials[2];
    cubeNode->position  = glm::vec3(-2, 1, 0);
    cubeNode->scale     = glm::vec3(1);
    cubeNode->rotation  = glm::angleAxis(3.141529f / 2.0f , glm::vec3(1.0f, 0.0f, 0.0f));

    // auto lightMeshNode = shared_ptr<MeshNode>(new MeshNode());
    // lightMeshNode->mesh      = DemoData::meshes[0];
    // lightMeshNode->material  = DemoData::materials[0];
    // lightMeshNode->position  = glm::vec3(1, 2, 1);
    // lightMeshNode->scale     = glm::vec3(0.1);

    auto camNode = shared_ptr<CameraNode>(new CameraNode());
    camNode->name 		= "cam1";
    camNode->position 	= glm::vec3(0, 2, 5);
    camNode->target 	= glm::vec3(0, 0, 0);
    camNode->fov 		= 45;

    auto lightNode = make_shared<LightNode>(LightType::Spot);
    lightNode->name         = "light1";
    lightNode->position     = glm::vec3(-5, 5, 0);
    lightNode->spotTarget   = glm::vec3(0, 0, 0);
    lightNode->spotCutoff   = 50.0f;
    lightNode->spotExponent = 16.0f;
    lightNode->ambient = glm::vec4(0.1, 0.1, 0.1, 1.0);
    lightNode->diffuse = glm::vec4(0.5, 0.0, 0.0, 1.0);
    lightNode->specular = glm::vec4(0.0, 0.8, 0.1, 1.0);

    auto lightNode2 = make_shared<LightNode>(LightType::Spot);
    lightNode2->name         = "light2";
    lightNode2->position     = glm::vec3(5, 5, 0);
    lightNode2->spotTarget   = glm::vec3(0, 0, 0);
    lightNode2->spotCutoff   = 50.0f;
    lightNode2->spotExponent = 16.0f;
    lightNode2->ambient = glm::vec4(0.1, 0.1, 0.1, 1.0);
    lightNode2->diffuse = glm::vec4(0.5, 0.0, 0.0, 1.0);
    lightNode2->specular = glm::vec4(0.0, 0.8, 0.1, 1.0);

    scene->tree->add(planeNode);
    scene->tree->add(meshNode);
    scene->tree->add(cubeNode);
    // scene->tree->add(lightMeshNode);

    scene->tree->add(camNode);

    scene->tree->add(lightNode);
    scene->tree->add(lightNode2);


    auto camPosTrack = shared_ptr<Vec3Track>(new Vec3Track);
    camPosTrack->controlledObjects.push_back(&camNode->position[0]);
    camPosTrack->addKey(Key::vec3Key(0, glm::vec3(0, 1, 2)));
    camPosTrack->addKey(Key::vec3Key(0.5, glm::vec3(1, 1, 2)));
    camPosTrack->addKey(Key::vec3Key(1, glm::vec3(2, 1, 1)));

    auto timeline = shared_ptr<Timeline>(new Timeline);
    scene->timeline = timeline;
    scene->timeline->tracks.push_back(camPosTrack);

    // --------------------------------------------------------------------------------------
    // Run demo
    // --------------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < DEMO_LENGTH_IN_SECONDS) {

        double elapsedSeconds = timer.secondsSinceStart();

		// meshNode->position  = glm::vec3(0, sin(2*M_PI*0.25* elapsedSeconds), 0);
        meshNode->position = glm::vec3(0, 0.001, 0);
  //       camNode->target = glm::vec3(0, 0.0001, 0);
		// camNode->position = glm::vec3(0, 5, -10);
        lightNode2->position = glm::vec3(5 * sin(2*M_PI*0.25* elapsedSeconds), 2, 5 * cos(2*M_PI*0.25* elapsedSeconds));

        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene.process(demoPartScene.normalizeTime(elapsedSeconds));

        window.present();
    }

    return 0;
}