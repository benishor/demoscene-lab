#pragma once

#include <vector>
#include <Mesh.h>
#include <PointLight.h>

struct Scene {

    std::vector<Mesh> meshes;
    std::vector<PointLight> pointLights;
};