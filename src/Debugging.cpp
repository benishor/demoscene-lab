#include <Debugging.h>
#include <GLheaders.h>

void drawNormals(const Mesh& mesh) {
    glBegin(GL_LINES);
    for (auto& f : mesh.facets) {
        Vector3 centerPoint = (mesh.vertices[f.a].position +
                               mesh.vertices[f.b].position +
                               mesh.vertices[f.c].position) / 3.0;
        Vector3 endPoint = centerPoint + f.normal * 0.1;

        glColor4fv(mesh.material.diffuse.array);
        glVertex3d(centerPoint.x, centerPoint.y, centerPoint.z);

        glColor4fv(mesh.material.diffuse.array);
        glVertex3d(endPoint.x, endPoint.y, endPoint.z);
    }
    glEnd();
}

