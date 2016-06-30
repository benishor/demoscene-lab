#pragma once

#include <Material.h>
#include <Mesh.h>
#include <SceneNode.h>

namespace Acidrain {
    using namespace std;

    struct MeshNode : SceneNode {
        MeshNode();

        shared_ptr<Mesh> mesh;
        shared_ptr<Material> material;
    };
} // namespace Acidrain

