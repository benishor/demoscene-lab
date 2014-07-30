#pragma once

#include <memory>
#include <SceneTree.h>
#include <Timeline.h>

namespace Acidrain {

struct Scene {

    Scene();

    std::shared_ptr<SceneTree>  tree;
    std::shared_ptr<Timeline>   timeline;

    void    transform();
};

} // namespace Acidrain