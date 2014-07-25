#include <Mesh.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


Mesh::Mesh() {}

Mesh::~Mesh() {
    if (vboId > 0)
        glDeleteBuffers(1, &vboId);

    if (vboData != nullptr)
        delete [] vboData;
}

void Mesh::calculateNormals() {
    for (auto& vertex : vertices)
        vertex.normal = vec3(0);

    for (auto& facet : facets) {
        glm::vec3 a = vertices[facet.b].position - vertices[facet.a].position;
        glm::vec3 b = vertices[facet.c].position - vertices[facet.a].position;
        facet.normal = normalize(glm::cross(a, b));

        vertices[facet.a].normal += facet.normal;
        vertices[facet.b].normal += facet.normal;
        vertices[facet.c].normal += facet.normal;
    }

    for (auto& vertex : vertices)
        vertex.normal = normalize(vertex.normal);
}

void Mesh::computeModelToWorldMatrix() {
    modelToWorldMatrix = translate(position) * toMat4(rotation) * glm::scale(scale);
}

constexpr char* BUFFER_OFFSET(int i) {
    return (char*)NULL + i * sizeof(float);
}

constexpr int componentsInVertex() {
    return 3 + 3 + 2; // position, normal, uv
}

constexpr int sizeOfVertexInBytes() {
    return componentsInVertex() * sizeof(float);
}

void Mesh::draw() {
    if (vboId == 0) {
        vboData = new float[componentsInVertex() * facets.size() * 3];

        int i = 0;
        for (auto& f : facets) {
            for (int j = 0; j < 3; j++) {
                vboData[i++] = vertices[f.vertices[j]].position.x;
                vboData[i++] = vertices[f.vertices[j]].position.y;
                vboData[i++] = vertices[f.vertices[j]].position.z;
                vboData[i++] = vertices[f.vertices[j]].normal.x;
                vboData[i++] = vertices[f.vertices[j]].normal.y;
                vboData[i++] = vertices[f.vertices[j]].normal.z;
                vboData[i++] = f.textCoords[j].x;
                vboData[i++] = f.textCoords[j].y;
            }
        }

        glGenBuffers(1, &vboId);
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeOfVertexInBytes() * facets.size() * 3, vboData, GL_DYNAMIC_DRAW);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVertexInBytes() * facets.size() * 3, vboData);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeOfVertexInBytes(), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeOfVertexInBytes(), BUFFER_OFFSET(3));
    glTexCoordPointer(2, GL_FLOAT, sizeOfVertexInBytes(), BUFFER_OFFSET(6));

    glDrawArrays(GL_TRIANGLES, 0, facets.size() * 3);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}