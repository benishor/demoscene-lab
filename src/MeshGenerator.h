#pragma once

#include <memory>
#include <Mesh.h>
#include <TextureGenerator.h>

namespace Acidrain {

struct MeshGenerator {
    static std::shared_ptr<Mesh> cube();
    static std::shared_ptr<Mesh> sphere(int longitudePoints, int latitudePoints);
    static std::shared_ptr<Mesh> grid(int xSegments, int ySegments);
    static std::shared_ptr<Mesh> cylinder(int xSegments, int ySegments, bool capTop, bool capBottom);
};

void mapXform(std::shared_ptr<Mesh> mesh, TextureGenerator& texgen, unsigned char layer, unsigned char channel, float effectIntensity);

} // namespace Acidrain

