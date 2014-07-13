#pragma once

#include <vector>
#include <Vector3.h>
#include <Material.h>

struct Vertex {
    Vector3 position;
};

struct Facet {
    int a, b, c;
    Vector3 normal;
};

struct Edge {
    int from;
    int to;
};

class Mesh {
public:

    void calculateNormals();

    Vector3 position;
    Vector3 rotationAxis;
    double rotationAngle = 0;

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<Edge> edges;

    Material material;
};
