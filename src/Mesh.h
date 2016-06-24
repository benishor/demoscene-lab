#pragma once

#include <GLheaders.h>
#include <GLMheaders.h>
#include <vector>

namespace Acidrain {

    using namespace std;
    using namespace glm;

    struct Vertex {
        vec3 position;
        vec3 normal;
    };

    struct Facet {
        union {
            struct {
                int a, b, c;
            };
            int vertices[3];
        };
        vec3 normal;
        vec2 textCoords[3];
    };

    struct Edge {
        int from;
        int to;
    };

    struct Mesh {
        Mesh();
        ~Mesh();

        vector<Vertex> vertices;
        vector<Facet> facets;
        vector<Edge> edges;

        void render(bool flatShaded);

        bool initialized = false;
        GLuint vboId = 0;
        GLuint vaoId = 0;
        float* vboData = nullptr;
        int vboSizeInElements = 0;
    };

    void calculateNormals(Mesh& mesh);

} // namespace Acidrain