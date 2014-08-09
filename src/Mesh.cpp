#include <Mesh.h>

namespace Acidrain {

constexpr char* BUFFER_OFFSET(int i) {
    return (char*)NULL + i * sizeof(float);
}

constexpr int componentsInVertex() {
    return 3 + 3 + 2; // position, normal, uv
}

constexpr int sizeOfVertexInBytes() {
    return componentsInVertex() * sizeof(float);
}

void Mesh::render() {
    if (vboId == 0) {
        glGenBuffers(1, &vboId);
    }

    // construct data
    if ((facets.size() * 3) > vboSizeInElements) {

        if (vboData != nullptr)
            delete [] vboData;

        vboData = new float[componentsInVertex() * facets.size() * 3];
        vboSizeInElements = facets.size() * 3;
    }

    // fill in buffer
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


    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertexInBytes() * facets.size() * 3, vboData, GL_DYNAMIC_DRAW);

    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVertexInBytes() * facets.size() * 3, vboData);

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


void calculateNormals(Mesh& mesh) {
    for (auto& vertex : mesh.vertices)
        vertex.normal = glm::vec3(0);

    for (auto& facet : mesh.facets) {
        glm::vec3 a = mesh.vertices[facet.b].position - mesh.vertices[facet.a].position;
        glm::vec3 b = mesh.vertices[facet.c].position - mesh.vertices[facet.a].position;
        facet.normal = glm::normalize(glm::cross(a, b));

        mesh.vertices[facet.a].normal += facet.normal;
        mesh.vertices[facet.b].normal += facet.normal;
        mesh.vertices[facet.c].normal += facet.normal;
    }

    for (auto& vertex : mesh.vertices)
        vertex.normal = glm::normalize(vertex.normal);
}


} // namespace Acidrain