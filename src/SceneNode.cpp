#include <SceneNode.h>
#include <MeshNode.h>
#include <CameraNode.h>
#include <LightNode.h>

namespace Acidrain {

void SceneNode::calculateLocalTransform() {
    localToParentSpaceMatrix = glm::translate(position)
                               * glm::translate(pivot)
                               * glm::toMat4(rotation)
                               * glm::translate(-pivot)
                               * glm::scale(scale);
}

void SceneNode::applyParentTransform(const glm::mat4& parentTransform) {
    modelToWorldSpaceMatrix = parentTransform * localToParentSpaceMatrix;

    for (auto& child : children)
        child->applyParentTransform(modelToWorldSpaceMatrix);
}


MeshNode& SceneNode::asMeshNode() {
    return *static_cast<MeshNode*>(this);
}

LightNode& SceneNode::asLightNode() {
    return *static_cast<LightNode*>(this);
}

CameraNode& SceneNode::asCameraNode() {
    return *static_cast<CameraNode*>(this);
}


};

