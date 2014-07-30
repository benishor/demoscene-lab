#pragma once

#include <Material.h>
#include <Mesh.h>
#include <SceneNode.h>

namespace Acidrain {

struct MeshNode : SceneNode {
    MeshNode();
    
    std::shared_ptr<Mesh> 		mesh;
    std::shared_ptr<Material>	material;
};


} // namespace Acidrain

