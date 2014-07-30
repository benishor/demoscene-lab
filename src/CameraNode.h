#pragma once

#include <SceneNode.h>

namespace Acidrain {

struct CameraNode : SceneNode {

    CameraNode();
    void calculateLocalTransform() override;

    glm::vec3   target;
    float       rollAngle;
    float       fov;
    glm::mat4   viewMatrix;
    glm::mat4   projectionMatrix;
};


} // namespace Acidrain

