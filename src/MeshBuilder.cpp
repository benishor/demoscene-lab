#include <MeshBuilder.h>
#include <iomanip>

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

            v.position = normalize({x, y, z});
            result.vertices.push_back(v);

            if (j > 0) {
                int currentVertexIndex = j * longitudePoints + i;
                int nextVertexIndex = j * longitudePoints + ((i + 1) % longitudePoints);
                int topVertexIndex = (j - 1) * longitudePoints + i;
                int topNextVertexIndex = (j - 1) * longitudePoints + ((i + 1) % longitudePoints);

                UV currentUV = {i * du, (j + 2)* dv};
                UV nextUV = {(i + 1)* du, (j + 2)* dv};
                UV topUV = {i * du, (j + 1)* dv};
                UV topNextUV = {(i + 1)* du, (j + 1)* dv};

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
    v.position = {0, 1.0, 0};
    result.vertices.push_back(v);

    int topVertexIndex = longitudePoints * latitudePoints;
    for (int i = 0; i < longitudePoints; i++) {
        int j = 0;

        int currentVertexIndex = i;
        int nextVertexIndex = (currentVertexIndex + 1) % longitudePoints;

        UV currentUV = {i * du, dv};
        UV nextUV = {(i + 1)* du, dv};
        UV topUV = {(i + 0.5)* du, 0};

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
    v.position = {0, -1.0, 0};
    result.vertices.push_back(v);

    int bottomVertexIndex = topVertexIndex + 1;
    for (int i = 0; i < longitudePoints; i++) {
        int j = latitudePoints;

        int currentVertexIndex = i + (latitudePoints - 1) * longitudePoints;
        int nextVertexIndex = (i + 1) % longitudePoints + (latitudePoints - 1) * longitudePoints;

        UV currentUV = {i * du, 1.0 - dv};
        UV nextUV = {(i + 1)* du, 1.0 - dv};
        UV bottomUV = {(i + 0.5)* du, 1.0};

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
    result.vertices = {
        { -0.5, -0.5, -0.5},
        { 0.5, -0.5, -0.5},
        { 0.5,  0.5, -0.5},
        { -0.5,  0.5, -0.5},

        { -0.5, -0.5,  0.5},
        { 0.5, -0.5,  0.5},
        { 0.5,  0.5,  0.5},
        { -0.5,  0.5,  0.5},
    };

    result.edges = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

    result.facets = {
        {0, 2, 1},
        {2, 0, 3},
        {5, 7, 4},
        {7, 5, 6},
        {1, 6, 5},
        {6, 1, 2},
        {4, 3, 0},
        {3, 4, 7},
        {4, 1, 5},
        {1, 4, 0},
        {3, 6, 2},
        {6, 3, 7}
    };
    result.calculateNormals();
    return result;
}
