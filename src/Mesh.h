#pragma once

#include <GLheaders.h>
#include <GLMheaders.h>
#include <vector>

namespace Acidrain {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Facet {
    union {
        struct {
            int a, b, c;
        };
        int vertices[3];
    };
    glm::vec3   normal;
    glm::vec2   textCoords[3];
};

struct Edge {
    int from;
    int to;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Facet>  facets;
    std::vector<Edge>   edges;

    void render();

    GLuint vboId = 0;
    float* vboData = nullptr;
    int vboSizeInElements = 0;
};

void calculateNormals(Mesh& mesh);

} // namespace Acidrain