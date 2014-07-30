#include <MeshGenerator.h>

namespace Acidrain {

void addVertex(Mesh& m, float x, float y, float z) {
    Vertex v;
    v.position.x = x;
    v.position.y = y;
    v.position.z = z;
    m.vertices.push_back(v);
}

void addFacet(Mesh& m, int a, int b, int c, float u1, float v1, float u2, float v2, float u3, float v3, bool edge1Visible, bool edge2Visible, bool edge3Visible) {
    Facet f;
    f.a = a; f.b = b; f.c = c;
    f.textCoords[0] = glm::vec2(u1, v1);
    f.textCoords[1] = glm::vec2(u2, v2);
    f.textCoords[2] = glm::vec2(u3, v3);
    m.facets.push_back(f);

    if (edge1Visible) m.edges.push_back({a, b});
    if (edge2Visible) m.edges.push_back({b, c});
    if (edge3Visible) m.edges.push_back({c, a});
}

std::shared_ptr<Mesh> MeshGenerator::cube() {
    Mesh* result = new Mesh;

    addVertex(*result, -0.5, -0.5, -0.5);
    addVertex(*result, 0.5, -0.5, -0.5);
    addVertex(*result, 0.5,  0.5, -0.5);
    addVertex(*result, -0.5,  0.5, -0.5);

    addVertex(*result, -0.5, -0.5,  0.5);
    addVertex(*result, 0.5, -0.5,  0.5);
    addVertex(*result, 0.5,  0.5,  0.5);
    addVertex(*result, -0.5,  0.5,  0.5);

    // back
    addFacet(*result, 0, 2, 1, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 2, 0, 3, 0, 1, 1, 0, 1, 1, false, true, true);

    // front
    addFacet(*result, 5, 7, 4, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 7, 5, 6, 0, 1, 1, 0, 1, 1, false, true, true);

    // right side
    addFacet(*result, 1, 6, 5, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 6, 1, 2, 0, 1, 1, 0, 1, 1, false, true, true);

    // left side
    addFacet(*result, 4, 3, 0, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 3, 4, 7, 0, 1, 1, 0, 1, 1, false, true, true);

    // bottom
    addFacet(*result, 4, 1, 5, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 1, 4, 0, 0, 1, 1, 0, 1, 1, false, true, true);

    // top
    addFacet(*result, 3, 6, 2, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(*result, 6, 3, 7, 0, 1, 1, 0, 1, 1, false, true, true);

    calculateNormals(*result);
    return std::shared_ptr<Mesh>(result);
}


} // namespace Acidrain
