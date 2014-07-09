#include <Mesh.h>
#include <GLheaders.h>

void Mesh::applyTransformation() {
    glTranslatef(position.x, position.y, position.z);
    glRotated(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

void Mesh::renderWireframe() {
    glBegin(GL_LINES);
    for (auto e : edges) {
        glVertex3f(vertices[e.from].x, vertices[e.from].y, vertices[e.from].z);
        glVertex3f(vertices[e.to].x, vertices[e.to].y, vertices[e.to].z);
    }
    glEnd();
}
