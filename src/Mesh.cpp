#include <Mesh.h>

#if defined (__APPLE__)
    #include <OpenGL/gl3.h>
#endif

namespace Acidrain {

    using namespace glm;

    constexpr char* BUFFER_OFFSET(int i) {
        return (char*) NULL + i * sizeof(float);
    }

    constexpr int componentsInVertex() {
        return 3 + 3 + 2; // position, normal, uv
    }

    constexpr int sizeOfVertexInBytes() {
        return componentsInVertex() * sizeof(float);
    }

    Mesh::Mesh() {

    }

    Mesh::~Mesh() {
        if (initialized) {
            glDeleteVertexArrays(1, &vaoId);
            glDeleteBuffers(1, &vboId);
        }
    }

    void Mesh::render(bool flatShaded) {
        if (!initialized) {
            glGenVertexArrays(1, &vaoId);
            glGenBuffers(1, &vboId);
            initialized = true;
        }

        // construct data
        if ((facets.size() * 3) > vboSizeInElements) {

            if (vboData != nullptr)
                delete[] vboData;

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
                if (flatShaded) {
                    vboData[i++] = f.normal.x;
                    vboData[i++] = f.normal.y;
                    vboData[i++] = f.normal.z;
                } else {
                    vboData[i++] = vertices[f.vertices[j]].normal.x;
                    vboData[i++] = vertices[f.vertices[j]].normal.y;
                    vboData[i++] = vertices[f.vertices[j]].normal.z;
                }
                vboData[i++] = f.textCoords[j].x;
                vboData[i++] = f.textCoords[j].y;
            }
        }

        glBindVertexArray(vaoId);
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeOfVertexInBytes() * facets.size() * 3, vboData, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0); // vertices on stream 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertexInBytes(), BUFFER_OFFSET(0));

        glEnableVertexAttribArray(1); // normals on stream 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeOfVertexInBytes(), BUFFER_OFFSET(3));

        glEnableVertexAttribArray(2); // text coords on stream 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertexInBytes(), BUFFER_OFFSET(6));

        glDrawArrays(GL_TRIANGLES, 0, facets.size() * 3);

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void calculateNormals(Mesh& mesh) {
        for (auto& vertex : mesh.vertices)
            vertex.normal = vec3(0);

        for (auto& facet : mesh.facets) {
            vec3 a = mesh.vertices[facet.b].position - mesh.vertices[facet.a].position;
            vec3 b = mesh.vertices[facet.c].position - mesh.vertices[facet.a].position;
            facet.normal = cross(a, b);
            if (length(facet.normal) > 0.00001) {
                facet.normal = normalize(facet.normal);
            }

            mesh.vertices[facet.a].normal += facet.normal;
            mesh.vertices[facet.b].normal += facet.normal;
            mesh.vertices[facet.c].normal += facet.normal;
        }

        for (auto& vertex : mesh.vertices)
            vertex.normal = normalize(vertex.normal);
    }

} // namespace Acidrain