#pragma once

#include <DemoPart.h>
#include <memory>
#include <string>
#include <Scene.h>

namespace Acidrain {

struct DemoPartScene : DemoPart {
    std::shared_ptr<Scene>      scene;
    std::string                 cameraName;
    float                       animationStartTime; // normalized time, [0-1]
    float                       animationEndTime;   // normalized time, [0-1]

    void process(float normalizedTime) override;
};


} // namespace Acidrain

