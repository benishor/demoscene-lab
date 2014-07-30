#include <Window.h>
#include <Timer.h>
#include <DemoEngine.h>

const char* vs = R"(
	uniform mat4 mmtx;
	uniform mat4 vmtx;
	uniform mat4 pmtx;

    void main() {   
        gl_Position    = pmtx * vmtx * mmtx * gl_Vertex;
        gl_FrontColor  = gl_Color;
        gl_TexCoord[0] = gl_MultiTexCoord0;
    } 
)";

const char* ps = R"(
    uniform sampler2D mapDiffuse;
    uniform sampler2D mapSpecular;
    uniform float dptimen;
    void main (void) {
        gl_FragColor = texture2D(mapDiffuse, gl_TexCoord[0].st + vec2(dptimen)) * 0.5 +  texture2D(mapSpecular, gl_TexCoord[0].st - vec2(dptimen/2.0));
    }
)";


using namespace Acidrain;
using namespace std;

int main() {

    Window window(800, 600, WindowType::Windowed);

    // --------------------------------------------------------------------------------------
    // Load resources
    // --------------------------------------------------------------------------------------

    DemoData::textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 16, glm::vec4(1, 1, 1, 0.9), glm::vec4(0, 0.3, 0.1, 0.9))
        .getTexture(0)
    );
    DemoData::textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 64, glm::vec4(1, 0.2, 0.2, 0.9), glm::vec4(0.1, 0.1, 0.3, 0.9))
        .getTexture(0)
    );
    DemoData::shaders.push_back(shared_ptr<Shader>(new Shader(vs, ps)));
    DemoData::meshes.push_back(MeshGenerator::cube());

    shared_ptr<Material> material = shared_ptr<Material>(new Material());
    material->shader = DemoData::shaders[0];
    material->textures[TextureRole::Diffuse] = DemoData::textures[0];
    material->textures[TextureRole::Specular] = DemoData::textures[1];

    DemoData::materials.push_back(material);

    // --------------------------------------------------------------------------------------
    // Set up demo parts
    // --------------------------------------------------------------------------------------

    DemoPartClear demoPartClear;
    demoPartClear.startTime = 0;
    demoPartClear.endTime = 4;
    demoPartClear.color = glm::vec4(0.1, 0, 0.2, 1);
    
    DemoPartScene demoPartScene;
    demoPartScene.startTime = 0;
    demoPartScene.endTime = 4;
    demoPartScene.cameraName = "cam1";

    Scene* scene = new Scene();
    demoPartScene.scene = shared_ptr<Scene>(scene);

    MeshNode* meshNode = new MeshNode();
    meshNode->mesh 		= DemoData::meshes[0];
    meshNode->material 	= DemoData::materials[0];
    meshNode->position  = glm::vec3(0, 0, 0);

    MeshNode* meshNode2 = new MeshNode();
    meshNode2->mesh      = DemoData::meshes[0];
    meshNode2->material  = DemoData::materials[0];
    meshNode2->position  = glm::vec3(-1.5, 0, 0);

    MeshNode* meshNode3 = new MeshNode();
    meshNode3->mesh      = DemoData::meshes[0];
    meshNode3->material  = DemoData::materials[0];
    meshNode3->position  = glm::vec3(1.5, 0, 0);

    CameraNode* camNode = new CameraNode();
    camNode->name 		= "cam1";
    camNode->position 	= glm::vec3(0, 1, 1.5);
    camNode->target 	= glm::vec3(0);
    camNode->fov 		= 45;

    CameraNode* camNode2 = new CameraNode();
    camNode2->name       = "cam2";
    camNode2->position   = glm::vec3(0, 0, 3);
    camNode2->target     = glm::vec3(0);
    camNode2->fov        = 45;

    LightNode* lightNode = new LightNode();
    lightNode->position  = glm::vec3(0, 0, 1);
    lightNode->lightType = LightType::Point;

    scene->tree->add(shared_ptr<SceneNode>(meshNode));
    scene->tree->add(shared_ptr<SceneNode>(meshNode2));
    scene->tree->add(shared_ptr<SceneNode>(meshNode3));
    scene->tree->add(shared_ptr<SceneNode>(camNode));
    scene->tree->add(shared_ptr<SceneNode>(camNode2));
    scene->tree->add(shared_ptr<SceneNode>(lightNode));

    // Position Track
    auto posTrack = shared_ptr<AnimationTrack>(new Vec3Track());
    scene->timeline->tracks.push_back(posTrack);

    posTrack->addControlledObject(&meshNode->position);

    posTrack->addKey(Key::vec3Key(0.0, glm::vec3(0, 0, 0)));
    posTrack->addKey(Key::vec3Key(0.5, glm::vec3(0, 1, 0)));
    posTrack->addKey(Key::vec3Key(1.0, glm::vec3(0, 0, 0)));

    // Scale track
    auto scaleTrack = shared_ptr<AnimationTrack>(new FloatTrack());
    scene->timeline->tracks.push_back(scaleTrack);

    scaleTrack->addControlledObject(&meshNode->scale.x);
    scaleTrack->addControlledObject(&meshNode->scale.y);
    scaleTrack->addControlledObject(&meshNode->scale.z);

    scaleTrack->addKey(Key::floatKey(0.00, 1));
    scaleTrack->addKey(Key::floatKey(0.01, 2));
    scaleTrack->addKey(Key::floatKey(0.25, 1));
    scaleTrack->addKey(Key::floatKey(0.26, 2));
    scaleTrack->addKey(Key::floatKey(0.50, 1));
    scaleTrack->addKey(Key::floatKey(0.51, 2));
    scaleTrack->addKey(Key::floatKey(0.75, 1));
    scaleTrack->addKey(Key::floatKey(0.76, 2));
    scaleTrack->addKey(Key::floatKey(1.00, 1));

    // --------------------------------------------------------------------------------------
    // Run demo
    // --------------------------------------------------------------------------------------

    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < 4) {

        double elapsedSeconds = timer.secondsSinceStart();

        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene.process(demoPartScene.normalizeTime(elapsedSeconds));

        window.present();
    }

    return 0;
}