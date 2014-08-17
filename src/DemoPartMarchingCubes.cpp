#include <DemoPartMarchingCubes.h>
#include <MarchingCubesTable.h>
#include <MeshGenerator.h>
// #include <iostream>
// #include <iomanip>

namespace Acidrain {

DemoPartMarchingCubes::DemoPartMarchingCubes(std::shared_ptr<Mesh> affectedMesh) {
    mesh = affectedMesh;

    field.emitters.push_back(glm::vec4(0, 0.7, 0, 1));
    field.emitters.push_back(glm::vec4(0, -0.7, 0, 1));
    field.emitters.push_back(glm::vec4(0.3, -0.7, 0.3, 1));
    // field.emitters.push_back(glm::vec4(-0.5, 0.7, -0.3, 1));
}

void DemoPartMarchingCubes::process(float normalizedTime) {

    field.emitters[0].x = sin(normalizedTime * 2 * M_PI * 4);
    field.emitters[0].z = cos(normalizedTime * 2 * M_PI * 4);
    field.emitters[0].y = 0.7 * cos(normalizedTime * 2 * M_PI * 2);

    field.emitters[1].x = sin(normalizedTime * 2 * M_PI * 7);
    field.emitters[1].z = cos(normalizedTime * 2 * M_PI * 3.5);

    field.emitters[2].y = 0.5 * sin(normalizedTime * 2 * M_PI * 4);
    field.emitters[2].z = 0.8 * cos(normalizedTime * 2 * M_PI * 2);

    // field.emitters[3].y = 0.8 * cos(normalizedTime * 2 * M_PI * 3);
    // field.emitters[3].z = 0.2 * sin(normalizedTime * 2 * M_PI * 8);

    grid.evaluateForces(field);
    grid.triangulate(*mesh.get(), minFieldValue);

    for (auto& v : mesh->vertices)
        v.normal = field.getFieldNormalAt(v.position);

    // calculateNormals(*mesh.get());
}

void MarchingCubesGrid::evaluateForces(ForceField& field) {

    // Make sure we have enough space preallocated.
    // We use resize because we want it also logically allocated with defaults
    // so we can simply access an item at certain index with no issues
    gridVertices.resize((xCells + 1) * (yCells + 1) * (zCells + 1));

    glm::vec3 stepSize(
        (xMax - xMin) / static_cast<float>(xCells),
        (yMax - yMin) / static_cast<float>(yCells),
        (zMax - zMin) / static_cast<float>(zCells)
    );

    int cellIndex = 0;
    glm::vec4 gridVertex;

    // starts from top-left-farthest and ends to bottom-right-closest
    for (int z = 0; z <= zCells; z++) {
        gridVertex.z = zMin + z * stepSize.z;

        for (int y = 0; y <= yCells; y++) {
            gridVertex.y = yMax - y * stepSize.y;

            for (int x = 0; x <= xCells; x++) {
                gridVertex.x = xMin + x * stepSize.x;
                gridVertex.w = field.getFieldValueAt(glm::vec3(gridVertex));

                // std::cout << std::setprecision(5) << std::fixed << "(x: " << gridVertex.x << ", y: " << gridVertex.y << ", z: " << gridVertex.z << ", val: " << gridVertex.w << ")" << std::endl;

                gridVertices[cellIndex] = gridVertex;

                cellIndex++;
            }
        }
    }
}


void MarchingCubesGrid::triangulate(Mesh& mesh, float minFieldValue) {

    mesh.vertices.clear();
    mesh.facets.clear();
    mesh.edges.clear();

    for (int z = 0; z < zCells; z++) {
        for (int y = 0; y < yCells; y++) {
            for (int x = 0; x < xCells; x++) {

                calculateCellVertices(x, y, z);

                int cellType = classifyCell(minFieldValue);
                if (!edgeTable[cellType]) continue;

                calculateIntersectionVertices(cellType, minFieldValue);
                addResultingFacets(mesh, cellType);
            }
        }
    }
}


void MarchingCubesGrid::calculateCellVertices(int x, int y, int z) {
    // TODO: optimize these out. they are constant for a given grid size.
    int xVerts = xCells + 1;
    int yVerts = yCells + 1;
    int vertsInXYPlane = xVerts * yVerts;

    int leftTopFarthestIndex = z * vertsInXYPlane + x + y * xCells;

    // see http://paulbourke.net/geometry/polygonise/polygonise1.gif
    cubeVertexIndices[0] = leftTopFarthestIndex + xVerts;
    cubeVertexIndices[1] = leftTopFarthestIndex + xVerts + 1;
    cubeVertexIndices[2] = leftTopFarthestIndex + xVerts + 1 + vertsInXYPlane;
    cubeVertexIndices[3] = leftTopFarthestIndex + xVerts + vertsInXYPlane;

    cubeVertexIndices[4] = leftTopFarthestIndex;
    cubeVertexIndices[5] = leftTopFarthestIndex + 1;
    cubeVertexIndices[6] = leftTopFarthestIndex + 1 + vertsInXYPlane;
    cubeVertexIndices[7] = leftTopFarthestIndex + vertsInXYPlane;
}


int MarchingCubesGrid::classifyCell(float minFieldValue) {
    int result = 0;

    for (int i = 0; i < 8; i++)
        if (gridVertices[cubeVertexIndices[i]].w <= minFieldValue)
            result |= 1 << i;

    return result;
}


void MarchingCubesGrid::calculateIntersectionVertices(int cellType, float minFieldValue) {

    if (edgeTable[cellType] & 1) intersectionVertices[0] = intersection(cubeVertexIndices[0], cubeVertexIndices[1], minFieldValue);
    if (edgeTable[cellType] & 2) intersectionVertices[1] = intersection(cubeVertexIndices[1], cubeVertexIndices[2], minFieldValue);
    if (edgeTable[cellType] & 4) intersectionVertices[2] = intersection(cubeVertexIndices[2], cubeVertexIndices[3], minFieldValue);
    if (edgeTable[cellType] & 8) intersectionVertices[3] = intersection(cubeVertexIndices[3], cubeVertexIndices[0], minFieldValue);
    if (edgeTable[cellType] & 16) intersectionVertices[4] = intersection(cubeVertexIndices[4], cubeVertexIndices[5], minFieldValue);
    if (edgeTable[cellType] & 32) intersectionVertices[5] = intersection(cubeVertexIndices[5], cubeVertexIndices[6], minFieldValue);
    if (edgeTable[cellType] & 64) intersectionVertices[6] = intersection(cubeVertexIndices[6], cubeVertexIndices[7], minFieldValue);
    if (edgeTable[cellType] & 128) intersectionVertices[7] = intersection(cubeVertexIndices[7], cubeVertexIndices[4], minFieldValue);
    if (edgeTable[cellType] & 256) intersectionVertices[8] = intersection(cubeVertexIndices[0], cubeVertexIndices[4], minFieldValue);
    if (edgeTable[cellType] & 512) intersectionVertices[9] = intersection(cubeVertexIndices[1], cubeVertexIndices[5], minFieldValue);
    if (edgeTable[cellType] & 1024) intersectionVertices[10] = intersection(cubeVertexIndices[2], cubeVertexIndices[6], minFieldValue);
    if (edgeTable[cellType] & 2048) intersectionVertices[11] = intersection(cubeVertexIndices[3], cubeVertexIndices[7], minFieldValue);

}

glm::vec3 MarchingCubesGrid::intersection(int v1Index, int v2Index, float minFieldValue) {
    glm::vec4& a = gridVertices[v1Index];
    glm::vec4& b = gridVertices[v2Index];

    if (a.w != b.w) {
        float alpha = (minFieldValue - a.w) / (b.w - a.w);
        return glm::mix(glm::vec3(a), glm::vec3(b), alpha);
    } else {
        return glm::vec3(a);
    }
}


void MarchingCubesGrid::addResultingFacets(Mesh& mesh, int cellType) {
    Facet f;
    Vertex v;
    int vertexOffset = mesh.vertices.size();

    for (int n = 0; triTable[cellType][n] != -1; n += 3) {

        for (int i = 0; i < 3; i++) {
            v.position = intersectionVertices[triTable[cellType][n + 2 - i]];
            mesh.vertices.push_back(v);
        }

        f.a = vertexOffset++;
        f.b = vertexOffset++;
        f.c = vertexOffset++;
        mesh.facets.push_back(f);
    }
}


float ForceField::getFieldValueAt(const glm::vec3& position) const {
    float result = 0;

    for (auto& e : emitters) {
        float dist = glm::length(glm::vec3(e) - position);
        result += 1.0f / (dist * dist);
    }

    return result;
}


glm::vec3 ForceField::getFieldNormalAt(const glm::vec3& position) const {
    // Hello blackpawn! http://www.blackpawn.com/texts/metanormals/default.html
    glm::vec3 normal = glm::vec3(0);

    for (auto& e : emitters) {
        float distance = glm::length(glm::vec3(e) - position);
        float coefficient = 1.0f / (distance * distance * distance * distance);

        glm::vec3 temp = (glm::vec3(e) - position) * 2.0f;
        normal += temp * coefficient;
    }

    return glm::normalize(normal);
}

} // namespace Acidrain