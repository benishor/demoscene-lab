#include <DemoPartScene.h>
#include <MeshNode.h>
#include <CameraNode.h>
#include <LightNode.h>
#include <ShaderConstants.h>
#include <DemoData.h>

namespace Acidrain {

DemoPartScene::DemoPartScene() {
    renderer = std::make_shared<SceneRenderer>();
}

DemoPartScene::~DemoPartScene() {
}

void DemoPartScene::process(float normalizedTime) {
    if (scene->timeline)
        scene->timeline->setAt(normalizedTime);

    if (scene->tree) {
        scene->tree->transform();
        renderer->render(*scene->tree, cameraName);
    }
}

} // namespace Acidrain