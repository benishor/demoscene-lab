#include <Window.h>
#include <Timer.h>
#include <DemoEngine.h>

const char* vs = R"(
	uniform mat4 mmtx;
	uniform mat4 vmtx;
	uniform mat4 pmtx;
    varying vec3 normal;

    void main() {   
        gl_Position    = pmtx * vmtx * mmtx * gl_Vertex;
        gl_FrontColor  = gl_Color;
        gl_TexCoord[0] = gl_MultiTexCoord0;

        normal = gl_Normal;
    } 
)";

const char* ps = R"(
    uniform sampler2D mapDiffuse;
    uniform sampler2D mapSpecular;
    uniform float dptimen;
    void main (void) {
        gl_FragColor = texture2D(mapDiffuse, gl_TexCoord[0].st + vec2(dptimen))  * texture2D(mapSpecular, gl_TexCoord[0].st - vec2(dptimen/2.0));
    }
)";

const char* ps2 = R"(
    uniform sampler2D mapDiffuse;
    uniform sampler2D mapSpecular;
    uniform float dptimen;
    varying vec3 normal;

    void main (void) {
        // gl_FragColor = texture2D(mapDiffuse, gl_TexCoord[0].st + vec2(dptimen)) * 0.5 + texture2D(mapSpecular, gl_TexCoord[0].st - vec2(dptimen/2.0));
        // gl_FragColor = texture2D(mapDiffuse, gl_TexCoord[0].st);

        vec3 lightDir = vec3(0, 0, -1);
        float NdotL = max(dot(normalize(normal), lightDir), 0.0);

        vec4 diffuse = vec4(1, 1, 1, 1) * vec4(0.8, 0.8, 0.8, 1);
        vec4 col =  NdotL * diffuse;

        gl_FragColor = col;
    }
)";

const char* ps3 = R"(
    uniform sampler2D mapDiffuse;
    void main (void) {
        gl_FragColor = texture2D(mapDiffuse, gl_TexCoord[0].st);
    }
)";


const char* vsEnvMap = R"(
    uniform mat4 mmtx;
    uniform mat4 vmtx;
    uniform mat4 pmtx;

    void main() {   
        vec3 u = normalize( vec3(vmtx * mmtx * gl_Vertex) );
        vec3 n = normalize( mat3(vmtx * mmtx) * gl_Normal );
        vec3 r = reflect( u, n );
        float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
        gl_TexCoord[0].s = r.x/m + 0.5;
        gl_TexCoord[0].t = r.y/m + 0.5;

        gl_Position    = pmtx * vmtx * mmtx * gl_Vertex;
        gl_FrontColor  = gl_Color;
    } 
)";


const char* psEnvMap = R"(
    uniform sampler2D mapDiffuse;
    uniform sampler2D mapSpecular;

    void main (void) {
        gl_FragColor = 8.0 * (texture2D(mapDiffuse, gl_TexCoord[0].st) / 9.0f) + 1/9.0f;
    }
)";



using namespace Acidrain;
using namespace std;

const float DEMO_LENGTH_IN_SECONDS = 10;

int main() {

    Window window(1024, 768, WindowType::Windowed);

    // --------------------------------------------------------------------------------------
    // Load resources
    // --------------------------------------------------------------------------------------

    DemoData::fbos.push_back(
        shared_ptr<Fbo>(new Fbo(512, 512))
    );

    DemoData::textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 8, glm::vec4(1, 1, 1, 0.9), glm::vec4(0, 0.3, 0.1, 0.9))
        .getTexture(0)
    );
    DemoData::textures.push_back(
        TextureGenerator(256, 256)
        .checkerBoard(0, 64, glm::vec4(1, 0.2, 0.2, 0.9), glm::vec4(0.1, 0.1, 0.3, 0.9))
        .getTexture(0)
    );
    DemoData::textures.push_back(
        TextureGenerator(256, 256)
        .lens(0, 100)
        .getTexture(0)
    );

    DemoData::shaders.push_back(shared_ptr<Shader>(new Shader(vs, ps)));
    DemoData::shaders.push_back(shared_ptr<Shader>(new Shader(vs, ps2)));
    DemoData::shaders.push_back(shared_ptr<Shader>(new Shader(vsEnvMap, psEnvMap)));
    DemoData::shaders.push_back(shared_ptr<Shader>(new Shader(vs, ps3)));

    DemoData::meshes.push_back(MeshGenerator::cube());
    DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    // DemoData::meshes.push_back(MeshGenerator::grid(30, 30));
    // DemoData::meshes.push_back(MeshGenerator::cylinder(30, 30, false, false));
    // DemoData::meshes.push_back(MeshGenerator::sphere(30, 30));
    DemoData::meshes.push_back(MeshGenerator::cube());
    // DemoData::meshes.push_back(MeshGenerator::grid(30, 30));


    // for (int i = 0; i < 6; i++) {
    //     subdivide(DemoData::meshes[2]);
    // }

    // TextureGenerator texgen = TextureGenerator(256, 256);
    // // texgen.checkerBoard(0, 32, glm::vec4(1), glm::vec4(0));
    // // texgen.roll(0, 8, 8);
    // texgen.lens(0, 100);
    // mapXform(DemoData::meshes[2], texgen, 0, 0, -0.4);


    auto material = shared_ptr<Material>(new Material());
    material->shader = DemoData::shaders[0];
    material->textures[TextureRole::Diffuse] = DemoData::textures[0];
    material->textures[TextureRole::Specular] = DemoData::textures[1];
    material->zBufferWrite = false;
    material->zBufferTest  = true;
    material->transparent    = true;
    material->blendSrcFactor = BlendingConstant::SRC_ALPHA;
    material->blendDstFactor = BlendingConstant::ONE;

    DemoData::materials.push_back(material);

    auto material2 = shared_ptr<Material>(new Material());
    material2->shader = DemoData::shaders[1];
    material2->textures[TextureRole::Diffuse] = DemoData::textures[0];
    material2->textures[TextureRole::Specular] = DemoData::textures[1];
    // material2->cullFaces = true;
    material2->transparent  = false;
    material2->zBufferWrite = true;
    material2->zBufferTest  = true;

    DemoData::materials.push_back(material2);


    auto material3 = shared_ptr<Material>(new Material());
    material3->shader = DemoData::shaders[2];
    material3->textures[TextureRole::Diffuse] = DemoData::textures[2];
    material3->textures[TextureRole::Specular] = DemoData::textures[1];
    material3->transparent  = false;
    material3->zBufferWrite = true;
    material3->zBufferTest  = true;

    DemoData::materials.push_back(material3);

    auto material4 = shared_ptr<Material>(new Material());
    material4->shader = DemoData::shaders[3];
    material4->textures[TextureRole::Diffuse] = DemoData::fbos[0]->getTexture();
    material4->transparent  = false;
    material4->zBufferWrite = true;
    material4->zBufferTest  = true;

    DemoData::materials.push_back(material4);

    // --------------------------------------------------------------------------------------
    // Set up demo parts
    // --------------------------------------------------------------------------------------

    DemoPartClear demoPartClear;
    demoPartClear.startTime = 0;
    demoPartClear.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartClear.color = glm::vec4(0.1, 0, 0.2, 1);

    DemoPartMarchingCubes demoPartMarchingCubes(DemoData::meshes[2]);
    demoPartMarchingCubes.startTime = 0;
    demoPartMarchingCubes.endTime = DEMO_LENGTH_IN_SECONDS;

    DemoPartScene demoPartScene;
    demoPartScene.startTime = 0;
    demoPartScene.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartScene.cameraName = "cam1";

    Scene* scene = new Scene();
    demoPartScene.scene = shared_ptr<Scene>(scene);

    auto meshNode = shared_ptr<MeshNode>(new MeshNode());
    meshNode->mesh 		= DemoData::meshes[0];
    meshNode->material 	= DemoData::materials[3];
    meshNode->position  = glm::vec3(1.5, 0, 0);

    auto meshNode2 = shared_ptr<MeshNode>(new MeshNode());
    meshNode2->mesh      = DemoData::meshes[2];
    meshNode2->material  = DemoData::materials[1];
    meshNode2->position  = glm::vec3(0, 0, 0);
    meshNode2->scale     = glm::vec3(1);
    // meshNode2->rotation  = glm::angleAxis(2.25f , glm::vec3(1.0f, 0.0f, 0.0f));

    auto meshNode3 = shared_ptr<MeshNode>(new MeshNode());
    meshNode3->mesh      = DemoData::meshes[2];
    meshNode3->material  = DemoData::materials[0];
    meshNode3->position  = glm::vec3(0, 0, 0);
    meshNode3->rotation  = glm::angleAxis(2.25f , glm::vec3(1.0f, 0.0f, 0.0f));

    auto camNode = shared_ptr<CameraNode>(new CameraNode());
    camNode->name 		= "cam1";
    camNode->position 	= glm::vec3(0, 0.6, 1.5);
    camNode->target 	= glm::vec3(0, -0.2, 0);
    camNode->fov 		= 45;

    auto camNode2 = shared_ptr<CameraNode>(new CameraNode());
    camNode2->name       = "cam2";
    camNode2->position   = glm::vec3(0, -1, 1);
    camNode2->target     = glm::vec3(0);
    camNode2->fov        = 45;

    auto lightNode = shared_ptr<LightNode>(new LightNode());
    lightNode->position  = glm::vec3(0, 0, 1);
    lightNode->lightType = LightType::Point;

    // scene->tree->add(meshNode);
    scene->tree->add(meshNode2);
    // scene->tree->add(meshNode3);

    scene->tree->add(camNode);
    scene->tree->add(camNode2);
    scene->tree->add(lightNode);



    // SCENE 2
    DemoPartScene demoPartScene2;
    demoPartScene2.startTime = 0;
    demoPartScene2.endTime = DEMO_LENGTH_IN_SECONDS;
    demoPartScene2.cameraName = "cam1";

    Scene* scene2 = new Scene();
    demoPartScene2.scene = shared_ptr<Scene>(scene2);

    scene2->tree->add(meshNode);
    // scene2->tree->add(meshNode3);
    scene2->tree->add(camNode);

    // Render to texture
    DemoPartRenderToTexture demoPartRenderToTexture(0, RenderToTextureAction::Start);
    DemoPartRenderToTexture demoPartRenderToTextureStop(0, RenderToTextureAction::Stop);

    // Position Track
    // auto posTrack = shared_ptr<AnimationTrack>(new Vec3Track());
    // scene->timeline->tracks.push_back(posTrack);

    // posTrack->addControlledObject(&meshNode->position);

    // posTrack->addKey(Key::vec3Key(0.0, glm::vec3(0, 0, 0)));
    // posTrack->addKey(Key::vec3Key(0.5, glm::vec3(0, 1, 0)));
    // posTrack->addKey(Key::vec3Key(1.0, glm::vec3(0, 0, 0)));

    // Scale track
    // auto scaleTrack = shared_ptr<AnimationTrack>(new FloatTrack());
    // scene->timeline->tracks.push_back(scaleTrack);

    // scaleTrack->addControlledObject(&meshNode->scale.x);
    // scaleTrack->addControlledObject(&meshNode->scale.y);
    // scaleTrack->addControlledObject(&meshNode->scale.z);

    // scaleTrack->addControlledObject(&meshNode2->scale.x);
    // scaleTrack->addControlledObject(&meshNode2->scale.y);
    // scaleTrack->addControlledObject(&meshNode2->scale.z);

    // scaleTrack->addControlledObject(&camNode->position.z);

    // scaleTrack->addKey(Key::floatKey(0.00, 2));
    // scaleTrack->addKey(Key::floatKey(0.01, 1));
    // scaleTrack->addKey(Key::floatKey(0.25, 2));
    // scaleTrack->addKey(Key::floatKey(0.26, 1));
    // scaleTrack->addKey(Key::floatKey(0.50, 2));
    // scaleTrack->addKey(Key::floatKey(0.51, 1));
    // scaleTrack->addKey(Key::floatKey(0.75, 2));
    // scaleTrack->addKey(Key::floatKey(0.76, 1));
    // scaleTrack->addKey(Key::floatKey(1.00, 2));

    // Scale track
    auto isoSurfaceThresholdTrack = shared_ptr<AnimationTrack>(new FloatTrack());
    scene->timeline->tracks.push_back(isoSurfaceThresholdTrack);

    isoSurfaceThresholdTrack->addControlledObject(&demoPartMarchingCubes.minFieldValue);

    isoSurfaceThresholdTrack->addKey(Key::floatKey(0.00, 3));
    isoSurfaceThresholdTrack->addKey(Key::floatKey(0.30, 3));
    isoSurfaceThresholdTrack->addKey(Key::floatKey(1.00, 3));


    // Color track
    auto colorTrack = shared_ptr<AnimationTrack>(new FloatTrack());
    scene->timeline->tracks.push_back(colorTrack);

    colorTrack->addControlledObject(&demoPartClear.color.r);

    colorTrack->addKey(Key::floatKey(0.00, 0));
    colorTrack->addKey(Key::floatKey(0.50, 0.3));
    colorTrack->addKey(Key::floatKey(1.00, 0.1));

    // --------------------------------------------------------------------------------------
    // Run demo
    // --------------------------------------------------------------------------------------

    Timer timer;
    while (!window.shouldQuit() && timer.secondsSinceStart() < DEMO_LENGTH_IN_SECONDS) {

        double elapsedSeconds = timer.secondsSinceStart();

        demoPartRenderToTexture.process(demoPartRenderToTexture.normalizeTime(elapsedSeconds));

        demoPartMarchingCubes.process(demoPartMarchingCubes.normalizeTime(elapsedSeconds));


        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene.process(demoPartScene.normalizeTime(elapsedSeconds));

        demoPartRenderToTextureStop.process(demoPartRenderToTextureStop.normalizeTime(elapsedSeconds));

        demoPartClear.process(demoPartClear.normalizeTime(elapsedSeconds));
        demoPartScene2.process(demoPartScene2.normalizeTime(elapsedSeconds));


        window.present();
    }

    return 0;
}