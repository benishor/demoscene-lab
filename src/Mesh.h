#pragma once

#include <vector>
#include <Material.h>

#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;
using namespace std;

struct Vertex {
    vec3 position;
    vec3 normal;
};

struct Facet {
    int a, b, c;
    vec3 normal;
    vec2 textCoords[3];
};

struct Edge {
    int from;
    int to;
};

struct Mesh {

    void calculateNormals();
    void computeModelToWorldMatrix();

    vec3 position = vec3(0);
    vec3 scale = vec3(1);
    quat rotation;

    mat4 modelToWorldMatrix = mat4(1);

    vector<Vertex> vertices;
    vector<Facet> facets;
    vector<Edge> edges;

    Material material;
};
