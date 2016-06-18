#include <SceneTree.h>

namespace Acidrain {

std::shared_ptr<SceneNode> SceneTree::nodeById(int id) const {
    for (auto& node : nodes)
        if (node->id == id)
            return node;

    return std::shared_ptr<SceneNode>(nullptr);
}

std::shared_ptr<SceneNode> SceneTree::nodeByNameAndType(std::string name, SceneNodeType type) const {
    for (auto& node : nodes)
        if (node->name == name && node->type == type)
            return node;

    return std::shared_ptr<SceneNode>(nullptr);
}


void SceneTree::add(std::shared_ptr<SceneNode> node) {
    add(node, std::shared_ptr<SceneNode>(nullptr));
}

void SceneTree::add(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent) {
    nodes.push_back(node);
    node->parent = parent;

    if (parent)
        parent->children.push_back(node);
    else
        rootNodes.push_back(node);

    if (node->type == SceneNodeType::Light)
        lights.push_back(node);
}

void SceneTree::transform() {
    for (auto& node : nodes)
        node->calculateLocalTransform();

    for (auto& node : rootNodes)
        node->applyParentTransform(glm::mat4(1));
}


} // namespace Acidrain