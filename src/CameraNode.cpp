#include <CameraNode.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <DemoData.h>

namespace Acidrain {

CameraNode::CameraNode() {
    type = SceneNodeType::Camera;
}

void CameraNode::calculateLocalTransform() {
    SceneNode::calculateLocalTransform();

    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
    viewMatrix *= glm::lookAt(
                      glm::vec3(position.x, position.y, position.z),
                      glm::vec3(target.x, target.y, target.z),
                      glm::vec3(0, 1, 0)
                  );

    projectionMatrix = glm::perspective(
                           fov,
                           DemoData::WIDTH / static_cast<float>(DemoData::HEIGHT),
                           0.1f,
                           100.0f
                       );
}

} // namespace Acidrain
