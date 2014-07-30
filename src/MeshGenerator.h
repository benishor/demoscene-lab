#pragma once

#include <memory>
#include <Mesh.h>

namespace Acidrain {

struct MeshGenerator {
    static std::shared_ptr<Mesh> cube();
};

} // namespace Acidrain

