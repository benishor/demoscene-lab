#include <MeshBuilder.h>

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