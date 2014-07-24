#pragma once

#include <vector>
#include <Material.h>

#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Facet {
    int a, b, c;
    glm::vec3 normal;
    glm::vec2 textCoords[3];
};

struct Edge {
    int from;
    int to;
};

class Mesh {
public:

    void calculateNormals();

    glm::vec3 position;
    glm::vec3 rotationAxis;
    double rotationAngle = 0;

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<Edge> edges;

    Material material;
};
