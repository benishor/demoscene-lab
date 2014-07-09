#pragma once
#include <vector>

struct Vertex {
    double x, y, z;
};

struct Facet {
    int a, b, c;
};

struct Edge {
    int from;
    int to;
};

class Mesh {
public:

    void applyTransformation();
    void renderWireframe();

    Vertex position;
    Vertex rotationAxis;
    double rotationAngle = 0;

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<Edge> edges;
};
