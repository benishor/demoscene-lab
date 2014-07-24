#include <MeshBuilder.h>
#include <iomanip>
#include <glm/glm.hpp>

using namespace glm;

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
    f.textCoords[0] = vec2(u1, v1); 
    f.textCoords[1] = vec2(u2, v2); 
    f.textCoords[2] = vec2(u3, v3); 
    m.facets.push_back(f);

    if (edge1Visible) m.edges.push_back({a, b});
    if (edge2Visible) m.edges.push_back({b, c});
    if (edge3Visible) m.edges.push_back({c, a});
}


Mesh MeshBuilder::sphere(int longitudePoints, int latitudePoints) {
    Mesh result;

    Vertex v;
    Facet f;
    Edge edge;

    double x, y, z;

    double du = 1.0 / static_cast<double>(longitudePoints);
    double dv = 1.0 / static_cast<double>(latitudePoints + 1);

    for (int j = 0; j < latitudePoints; j++) {

        y = 1.0 - (j + 1) * 2.0 / static_cast<double>(latitudePoints + 1);
        double currentCircleRadius = sin(M_PI * (j + 1) / static_cast<double>(latitudePoints + 1));

        for (int i = 0; i < longitudePoints; i++) {

            x = currentCircleRadius * cos((i * 2.0 * M_PI) / static_cast<double>(longitudePoints));
            z = currentCircleRadius * sin((i * 2.0 * M_PI) / static_cast<double>(longitudePoints));

            v.position = glm::normalize(glm::vec3(x, y, z));
            result.vertices.push_back(v);

            if (j > 0) {
                int currentVertexIndex = j * longitudePoints + i;
                int nextVertexIndex = j * longitudePoints + ((i + 1) % longitudePoints);
                int topVertexIndex = (j - 1) * longitudePoints + i;
                int topNextVertexIndex = (j - 1) * longitudePoints + ((i + 1) % longitudePoints);

                vec2 currentUV = vec2(i * du, (j + 2)* dv);
                vec2 nextUV = vec2((i + 1)* du, (j + 2)* dv);
                vec2 topUV = vec2(i * du, (j + 1)* dv);
                vec2 topNextUV = vec2((i + 1)* du, (j + 1)* dv);

                f.a = currentVertexIndex;
                f.b = topVertexIndex;
                f.c = nextVertexIndex;

                f.textCoords[0] = currentUV;
                f.textCoords[1] = topUV;
                f.textCoords[2] = nextUV;

                result.facets.push_back(f);

                f.a = nextVertexIndex;
                f.b = topVertexIndex;
                f.c = topNextVertexIndex;

                f.textCoords[0] = nextUV;
                f.textCoords[1] = topUV;
                f.textCoords[2] = topNextUV;

                result.facets.push_back(f);

                edge.from = topVertexIndex;
                edge.to = topNextVertexIndex;
                result.edges.push_back(edge);

                edge.from = topVertexIndex;
                edge.to = currentVertexIndex;
                result.edges.push_back(edge);
            }
        }
    }

    // top capping
    v.position = glm::vec3(0, 1.0, 0);
    result.vertices.push_back(v);

    int topVertexIndex = longitudePoints * latitudePoints;
    for (int i = 0; i < longitudePoints; i++) {
        int j = 0;

        int currentVertexIndex = i;
        int nextVertexIndex = (currentVertexIndex + 1) % longitudePoints;

        vec2 currentUV = vec2(i * du, dv);
        vec2 nextUV = vec2((i + 1)* du, dv);
        vec2 topUV = vec2((i + 0.5)* du, 0);

        f.textCoords[0] = currentUV;
        f.textCoords[1] = nextUV;
        f.textCoords[2] = topUV;

        f.a = currentVertexIndex;
        f.c = nextVertexIndex;
        f.b = topVertexIndex;
        result.facets.push_back(f);

        edge.from = topVertexIndex;
        edge.to = currentVertexIndex;
        result.edges.push_back(edge);
    }

    // bottom capping
    v.position = glm::vec3(0, -1.0, 0);
    result.vertices.push_back(v);

    int bottomVertexIndex = topVertexIndex + 1;
    for (int i = 0; i < longitudePoints; i++) {
        int j = latitudePoints;

        int currentVertexIndex = i + (latitudePoints - 1) * longitudePoints;
        int nextVertexIndex = (i + 1) % longitudePoints + (latitudePoints - 1) * longitudePoints;

        vec2 currentUV = vec2(i * du, 1.0 - dv);
        vec2 nextUV = vec2((i + 1)* du, 1.0 - dv);
        vec2 bottomUV = vec2((i + 0.5)* du, 1.0);

        f.textCoords[0] = currentUV;
        f.textCoords[1] = bottomUV;
        f.textCoords[2] = nextUV;

        f.a = currentVertexIndex;
        f.c = bottomVertexIndex;
        f.b = nextVertexIndex;
        result.facets.push_back(f);

        edge.from = currentVertexIndex;
        edge.to = nextVertexIndex;
        result.edges.push_back(edge);

        edge.from = currentVertexIndex;
        edge.to = bottomVertexIndex;
        result.edges.push_back(edge);
    }

    result.calculateNormals();
    return result;
}

Mesh MeshBuilder::cube() {
    Mesh result;

    addVertex(result,-0.5, -0.5, -0.5);
    addVertex(result, 0.5, -0.5, -0.5);
    addVertex(result, 0.5,  0.5, -0.5);
    addVertex(result,-0.5,  0.5, -0.5);

    addVertex(result,-0.5, -0.5,  0.5);
    addVertex(result, 0.5, -0.5,  0.5);
    addVertex(result, 0.5,  0.5,  0.5);
    addVertex(result,-0.5,  0.5,  0.5);

    // back
    addFacet(result, 0, 2, 1, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 2, 0, 3, 0, 1, 1, 0, 1, 1, false, true, true);

    // front
    addFacet(result, 5, 7, 4, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 7, 5, 6, 0, 1, 1, 0, 1, 1, false, true, true);

    // right side
    addFacet(result, 1, 6, 5, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 6, 1, 2, 0, 1, 1, 0, 1, 1, false, true, true);

    // left side
    addFacet(result, 4, 3, 0, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 3, 4, 7, 0, 1, 1, 0, 1, 1, false, true, true);

    // bottom
    addFacet(result, 4, 1, 5, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 1, 4, 0, 0, 1, 1, 0, 1, 1, false, true, true);

    // top
    addFacet(result, 3, 6, 2, 1, 0, 0, 1, 0, 0, false, true, true);
    addFacet(result, 6, 3, 7, 0, 1, 1, 0, 1, 1, false, true, true);

    result.calculateNormals();
    return result;
}
