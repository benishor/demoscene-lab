#pragma once

#include <vector>
#include <Material.h>
#include <Mesh.h>
#include <Fbo.h>

namespace Acidrain {

struct DemoData {

    static std::vector<std::shared_ptr<Texture>>    textures;
    static std::vector<std::shared_ptr<Shader>>     shaders;
    static std::vector<std::shared_ptr<Material>>   materials;
    static std::vector<std::shared_ptr<Mesh>>       meshes;
    static std::vector<std::shared_ptr<Fbo>>        fbos;

    // holds the current viewport size at any point in time
    static int WIDTH;
    static int HEIGHT;
};

} // namespace Acidrain
