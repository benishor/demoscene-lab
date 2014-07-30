#pragma once

#include <vector>
#include <Material.h>
#include <Mesh.h>

namespace Acidrain {

struct DemoData {
    static std::vector<std::shared_ptr<Texture>>    textures;
    static std::vector<std::shared_ptr<Shader>>     shaders;
    static std::vector<std::shared_ptr<Material>>   materials;
    static std::vector<std::shared_ptr<Mesh>>       meshes;
};

} // namespace Acidrain
