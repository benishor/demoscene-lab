#pragma once

#include <memory>
#include <Mesh.h>
#include <TextureGenerator.h>

namespace Acidrain {

    using namespace std;

    struct MeshGenerator {
        static shared_ptr<Mesh> cube();
        static shared_ptr<Mesh> sphere(int longitudePoints, int latitudePoints);
        static shared_ptr<Mesh> grid(int xSegments, int ySegments);
        static shared_ptr<Mesh> cylinder(int xSegments, int ySegments, bool capTop, bool capBottom);
        static shared_ptr<Mesh> revolveObject(vector<vec2> contour, int rotationSteps);
        static shared_ptr<Mesh> extrude(shared_ptr<Mesh> mesh,
                                        vector<int> facetsToExtrude,
                                        float extrusionHeight,
                                        int times);
        static shared_ptr<Mesh> cog(float wheelInnerDiameter,
                                    float wheelOuterDiameter,
                                    int wheelContourSegments,
                                    int numberOfTeeth,
                                    float toothHeight,
                                    float toothWidthBase,
                                    float toothWidthTop,
                                    float cogDepth);
        static int extrudeFace(Mesh* mesh, Facet facet, float amount, vec3 normalOffset);
    };

    void mapXform(shared_ptr<Mesh> mesh, TextureGenerator& texgen, unsigned char layer, unsigned char channel, float effectIntensity);
    void subdivide(shared_ptr<Mesh> mesh);

} // namespace Acidrain

