#pragma once

#include <SceneNode.h>
#include <string>

namespace Acidrain {

class SceneTree {
public:

    std::shared_ptr<SceneNode> nodeById(int id) const;
    std::shared_ptr<SceneNode> nodeByNameAndType(std::string name, SceneNodeType type) const;

    void add(std::shared_ptr<SceneNode> node);
    void add(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent);

    void transform();

    std::vector<std::shared_ptr<SceneNode>> nodes;
    std::vector<std::shared_ptr<SceneNode>> rootNodes;
    std::vector<std::shared_ptr<SceneNode>> lights;
};


} // namespace Acidrain