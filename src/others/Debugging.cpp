#include <Debugging.h>
#include <GLheaders.h>
#include <fstream>


// void drawNormals(const Mesh& mesh) {

//     static GLfloat whiteCol[4] = {1.f, 1.f, 1.f, 1.f};

//     glBegin(GL_LINES);
//     for (auto& f : mesh.facets) {
//         glm::vec3 centerPoint = (mesh.vertices[f.a].position +
//                                  mesh.vertices[f.b].position +
//                                  mesh.vertices[f.c].position) / 3.0f;
//         glm::vec3 endPoint = centerPoint + f.normal * 0.1f;

//         glColor4fv(whiteCol);
//         glVertex3d(centerPoint.x, centerPoint.y, centerPoint.z);

//         glColor4fv(whiteCol);
//         glVertex3d(endPoint.x, endPoint.y, endPoint.z);
//     }
//     glEnd();
// }

// void saveTexture(const Texture& texture) {
//     std::ofstream file("texture.raw", std::ios::out | std::ios::binary);
//     file.write(reinterpret_cast<const char*>(texture.getBuffer()), texture.getWidth() * texture.getHeight() * 4);
// }