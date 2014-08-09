#pragma once

#include <DemoPart.h>
#include <Mesh.h>
#include <memory>

namespace Acidrain {

struct ForceField {
    float getFieldValueAt(const glm::vec3& position) const;
    std::vector<glm::vec4> emitters; // vec4 = position + strength
};

struct MarchingCubesGrid {
    int xCells = 50;
    int yCells = 50;
    int zCells = 50;

    float xMin = -2;
    float xMax =  2;

    float yMin = -2;
    float yMax =  2;

    float zMin = -2;
    float zMax =  2;

    // .w will hold field value for code size optimisation purposes
    std::vector<glm::vec4> gridVertices;

    void            evaluateForces(ForceField& field);
    void            triangulate(Mesh& mesh, float minFieldValue);

private:
    void            calculateCellVertices(int x, int y, int z);
    int             classifyCell(float minFieldValue);
    void            calculateIntersectionVertices(int cellType, float minFieldValue);
    void            addResultingFacets(Mesh& mesh, int cellType);
    glm::vec3       intersection(int v1Index, int v2Index, float minFieldValue);

    int             cubeVertexIndices[8];
    glm::vec3       intersectionVertices[12];

};

struct DemoPartMarchingCubes : DemoPart {
    DemoPartMarchingCubes(std::shared_ptr<Mesh> affectedMesh);

    void process(float normalizedTime) override;

    MarchingCubesGrid       grid;
    ForceField              field;
    float                   minFieldValue = 2.6;
    std::shared_ptr<Mesh>   mesh;
};


} // namespace Acidrain

