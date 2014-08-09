#pragma once

#include <DemoPart.h>
#include <Mesh.h>
#include <memory>

namespace Acidrain {

struct ForceField {
    float getFieldValueAt(const glm::vec3& position) const;
};

struct MarchingCubesGrid {
    int xCells = 20;
    int yCells = 20;
    int zCells = 20;

    float xMin = -1;
    float xMax =  1;

    float yMin = -1;
    float yMax =  1;

    float zMin = -1;
    float zMax =  1;

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

