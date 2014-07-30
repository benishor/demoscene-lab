#pragma once

#include <vector>
#include <memory>
#include <GLMheaders.h>

namespace Acidrain {

enum class SceneNodeType {
    Mesh,
    Camera,
    Light
};

struct MeshNode;
struct LightNode;
struct CameraNode;

struct SceneNode {
    virtual void calculateLocalTransform();
    void applyParentTransform(const glm::mat4& parentTransform);

    MeshNode&   asMeshNode();
    LightNode&  asLightNode();
    CameraNode& asCameraNode();

    int             id;
    std::string     name;
    SceneNodeType   type; // We rely on type comparison instead of dynamic casting because comparing types is much faster

    // Helpers to navigate the hierarchy
    std::shared_ptr<SceneNode> parent;
    std::vector<std::shared_ptr<SceneNode>> children;

    // Transform
    glm::vec3 scale    = glm::vec3(1);
    glm::vec3 position = glm::vec3(0);
    glm::vec3 pivot    = glm::vec3(0);
    glm::quat rotation;

    glm::mat4 localToParentSpaceMatrix = glm::mat4(1);
    glm::mat4 modelToWorldSpaceMatrix  = glm::mat4(1);
};

} // namespace Acidrain