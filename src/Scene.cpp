#include <Scene.h>

namespace Acidrain {

Scene::Scene() {
    tree = std::shared_ptr<SceneTree>(new SceneTree());
    timeline = std::shared_ptr<Timeline>(new Timeline());
}

} // namespace Acidrain