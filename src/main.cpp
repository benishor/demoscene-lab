#include <Window.h>
#include <Timer.h>
#include <DemoEngine.h>

using namespace Acidrain;
using namespace std;
using namespace glm;

const float DEMO_LENGTH_IN_SECONDS = 100;

int main() {

//    Window window(1920, 1200, WindowType::Fullscreen);
    Window window(1024, 768, WindowType::Windowed);

    // --------------------------------------------------------------------------------------
    // Load resources
    // --------------------------------------------------------------------------------------

    TextureGenerator tg(256, 256);
//    tg.checkerBoard(0, 20, vec4(1), vec4(0));
    tg.brick(0, 50, 20, 4, vec4(1), vec4(0));
//    tg.polarGrid(0);

    shared_ptr<Mesh> affectedSphere = MeshGenerator::sphere(50, 50);
    mapXform(affectedSphere, tg, 0, 0, 0.2f);
    calculateNormals(*affectedSphere.get());

    affectedSphere = MeshGenerator::cog(0.2, 0.5, 50, 4, 0.1, 0.2, 0.2, 0.1);

    DemoData::meshes.push_back(affectedSphere);
    DemoData::meshes.push_back(MeshGenerator::sphere(50, 50));
    DemoData::meshes.push_back(MeshGenerator::grid(30, 30));
    DemoData::meshes.push_back(MeshGenerator::cube());

    auto material = shared_ptr<Material>(new Material());
    material->ambient = glm::vec4(0, 0, 0.15, 1);
    material->diffuse = glm::vec4(1, 1, 1, 1);
//    material->diffuse = glm::vec4(1, 0, 0, 1);
    material->specular = glm::vec4(0.3, 0.3, 0.3, 1);
    material->shininess = 200;
    material->castsShadows = true;

    TextureGenerator* textureGenerator = new TextureGenerator(256, 256);
//    material->textures[TextureRole::Diffuse] = textureGenerator->checkerBoard(0, 20, Colour(1, 1, 1, 1), Colour(0, 0.5, 0.2, 1)).getTexture(0);
    material->textures[TextureRole::Diffuse] = textureGenerator->brick(0, 50, 20, 4, Colour(0.3, 0.1, 0, 1), Colour(1, 0.3, 0, 1)).getTexture(0);
    DemoData::materials.push_back(material);

    auto material2 = make_shared<Material>();
    material2->ambient = glm::vec4(0, 0, 0.15, 1);
    material2->textures[TextureRole::Diffuse] = textureGenerator->getTexture(0);
//    material2->diffuse = glm::vec4(0.2, 1.0, 0.7, 1);
    material2->diffuse = glm::vec4(1, 1, 1, 1);
    material2->specular = glm::vec4(0.3, 0.3, 0.3, 1);
    material2->shininess = 200;
    material2->castsShadows = false;
    DemoData::materials.push_back(material2);

    auto material3 = make_shared<Material>();
    material3->ambient = glm::vec4(0, 0, 0.15, 1);
//    material3->diffuse = glm::vec4(1.0, 0.7, 0.2, 1);
    material3->diffuse = glm::vec4(1, 1, 1, 1);
    material3->specular = glm::vec4(0.3, 0.3, 0.3, 1);
    material3->shininess = 200;
    material3->castsShadows = true;
    DemoData::materials.push_back(material3);

    auto material4 = make_shared<Material>();
    material4->ambient = glm::vec4(0, 0, 0.15, 1);
//    material4->diffuse = glm::vec4(1.0, 0.7, 0.2, 1);
    material4->diffuse = glm::vec4(1, 1, 1, 1);
    material4->specular = glm::vec4(0.3, 0.3, 0.3, 1);
    material4->shininess = 200;
    material4->castsShadows = false;
    DemoData::materials.push_back(material4);

    auto material5 = make_shared<Material>();
    material5->ambient = glm::vec4(0.1, 0.1, 0.15, 1);
//    material4->diffuse = glm::vec4(1.0, 0.7, 0.2, 1);
    material5->diffuse = glm::vec4(1, 1, 1, 1);
    material5->specular = glm::vec4(1, 1, 1, 1);
    material5->shininess = 10;
    material5->castsShadows = false;
    DemoData::materials.push_back(material5);


    // --------------------------------------------------------------------------------------
    // Set up demo parts
    // --------------------------------------------------------------------------------------

    DemoPartClear demoPartClear;
    demoPartClear.startTime = 0;
    demoPartClear.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartClear.color = glm::vec4(0.1, 0.1, 0.1, 1);

    DemoPartScene demoPartScene;
    demoPartScene.startTime = 0;
    demoPartScene.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartScene.cameraName = "cam1";
    demoPartScene.lightName = "light1";

    Scene* scene = new Scene();
    demoPartScene.scene = shared_ptr<Scene>(scene);

    auto smallSphere = shared_ptr<MeshNode>(new MeshNode());
    smallSphere->mesh = DemoData::meshes[1];
    smallSphere->material = DemoData::materials[0];
    smallSphere->position = glm::vec3(0, 0, 0);
    smallSphere->scale = glm::vec3(0.5);
    // meshNode->rotation  = glm::angleAxis(2.25f , glm::vec3(1.0f, 0.0f, 0.0f));

    auto groundPlane = shared_ptr<MeshNode>(new MeshNode());
    groundPlane->mesh = DemoData::meshes[2];
    groundPlane->material = DemoData::materials[1];
    groundPlane->position = glm::vec3(0, 0, 0);
    groundPlane->scale = glm::vec3(10);
    groundPlane->rotation = glm::angleAxis(3.141529f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    auto backPlane = shared_ptr<MeshNode>(new MeshNode());
    backPlane->mesh = DemoData::meshes[2];
    backPlane->material = DemoData::materials[1];
    backPlane->position = vec3(0, 5, -5);
    backPlane->scale = vec3(10);
    backPlane->rotation = angleAxis(3.141529f, vec3(1.0f, 0.0f, 0.0f));

    auto leftPlane = shared_ptr<MeshNode>(new MeshNode());
    leftPlane->mesh = DemoData::meshes[2];
    leftPlane->material = DemoData::materials[1];
    leftPlane->position = vec3(-5, 5, 0);
    leftPlane->scale = vec3(10);
    leftPlane->rotation = angleAxis(-3.141529f / 2.0f, vec3(0.0f, 1.0f, 0.0f));

    auto rightPlane = shared_ptr<MeshNode>(new MeshNode());
    rightPlane->mesh = DemoData::meshes[2];
    rightPlane->material = DemoData::materials[1];
    rightPlane->position = vec3(5, 5, 0);
    rightPlane->scale = vec3(10);
    rightPlane->rotation = angleAxis(3.141529f / 2.0f, vec3(0.0f, 1.0f, 0.0f));

    auto bigSphere = make_shared<MeshNode>();
    bigSphere->mesh = DemoData::meshes[0];
    bigSphere->material = DemoData::materials[4];
    bigSphere->position = glm::vec3(-2, 2, 0);
    bigSphere->scale = glm::vec3(0.8);
    bigSphere->rotation = glm::angleAxis(3.141529f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    auto lightMeshNode = shared_ptr<MeshNode>(new MeshNode());
    lightMeshNode->mesh = DemoData::meshes[0];
    lightMeshNode->material = DemoData::materials[3];
    lightMeshNode->position = glm::vec3(1, 2, 1);
    lightMeshNode->scale = glm::vec3(0.1);

    auto camNode = shared_ptr<CameraNode>(new CameraNode());
    camNode->name = "cam1";
    camNode->position = glm::vec3(0, 2, 5);
    camNode->target = glm::vec3(0, 0, 0);
    camNode->fov = 45;

    auto lightNode = make_shared<LightNode>(LightType::Spot);
    lightNode->name = "light1";
    lightNode->position = glm::vec3(-5, 5, 0);
    lightNode->spotTarget = glm::vec3(0, 0, 0);
    lightNode->spotCutoff = 50.0f;
    lightNode->spotExponent = 16.0f;
    lightNode->ambient = glm::vec4(0.1, 0.1, 0.1, 1.0);
    lightNode->diffuse = glm::vec4(0.5, 1.0, 1.0, 1.0);
    lightNode->specular = glm::vec4(0.0, 0.8, 0.1, 1.0);

    auto lightNode2 = make_shared<LightNode>(LightType::Spot);
    lightNode2->name = "light2";
    lightNode2->position = glm::vec3(5, 5, 0);
    lightNode2->spotTarget = glm::vec3(0, 0, 0);
    lightNode2->spotCutoff = 50.0f;
    lightNode2->spotExponent = 16.0f;
    lightNode2->ambient = glm::vec4(0.1, 0.1, 0.1, 1.0);
    lightNode2->diffuse = glm::vec4(1.0, 0.5, 1.0, 1.0);
    lightNode2->specular = glm::vec4(0.0, 0.8, 0.1, 1.0);

    scene->tree->add(groundPlane);
    scene->tree->add(backPlane);
    scene->tree->add(leftPlane);
    scene->tree->add(rightPlane);

    scene->tree->add(smallSphere);
    scene->tree->add(bigSphere);
    scene->tree->add(lightMeshNode);

    scene->tree->add(camNode);

//    scene->tree->add(lightNode);
    scene->tree->add(lightNode2);

    // --------------------------------------------------------------------------------------
    // Run demo
    // --------------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < DEMO_LENGTH_IN_SECONDS) {

        double elapsedSeconds = timer.secondsSinceStart();

        bigSphere->mesh = MeshGenerator::extrude(MeshGenerator::sphere(50, 50), {200, 201, 100, 101, 400, 401}, 0.05, (int) ((sin(elapsedSeconds) + 0.5) * 40));

        smallSphere->position = glm::vec3(0, sin(2 * M_PI * 0.13 * elapsedSeconds) * 2.0 + 0.5, 0);
//        meshNode->position = glm::vec3(0, 0.001, 0);
        //       camNode->target = glm::vec3(0, 0.0001, 0);
        // camNode->position = glm::vec3(0, 5, -10);
        lightNode2->position = glm::vec3(5 * sin(2 * M_PI * 0.15 * elapsedSeconds), 2, 5 * cos(2 * M_PI * 0.15 * elapsedSeconds));
        lightNode->position = glm::vec3(5 * cos(2 * M_PI * 0.75 * elapsedSeconds), 2, 5 * sin(2 * M_PI * 0.75 * elapsedSeconds));

        lightMeshNode->position = lightNode2->position;

        bigSphere->rotation *= angleAxis((float)(M_PI/4.0 * timer.lap()), normalize(vec3(0.2, 0.5, 0.3)));

        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene.process(demoPartScene.normalizeTime(elapsedSeconds));

        window.present();
    }

    return 0;
}