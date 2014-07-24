#include <Camera.h>
#include <GLheaders.h>
#include <cstring>
#include <others/Debugging.h>

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// http://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/
// http://archive.gamedev.net/archive/reference/articles/article665.html
// http://www.cprogramming.com/tutorial/3d/rotationMatrices.html

// No more GLU. This is SGI implementation
// void lookAt(Vector3 position, Vector3 target, Vector3 up) {
//     int i;
//     GLfloat m[4][4];

//     Vector3 forward = normalize(target - position);

//     // Side = forward x up
//     Vector3 side = normalize(cross(forward, up));

//     // Recompute up as: up = side x forward
//     up = cross(side, forward);

//     // Make identity
//     m[3][0] = m[3][1] = m[3][2] = 0;
//     m[0][3] = m[1][3] = m[2][3] = 0;
//     m[3][3] = 1;

//     // Orientation part
//     m[0][0] = side.x;
//     m[1][0] = side.y;
//     m[2][0] = side.z;

//     m[0][1] = up.x;
//     m[1][1] = up.y;
//     m[2][1] = up.z;

//     m[0][2] = -forward.x;
//     m[1][2] = -forward.y;
//     m[2][2] = -forward.z;

//     // Move the world away from the position
//     m[0][3] = -position.x;
//     m[1][3] = -position.y;
//     m[2][3] = -position.z;

//     // glTranslated(-position.x, -position.y, -position.z);
//     glMultMatrixf(&m[0][0]);

//     GLfloat matrix[16];
//     glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
//     std::cout << "My way matrix: " << std::endl;
//     dumpMatrix(matrix, std::cout);
// }

void Camera::set() {
    using namespace glm;

    mat4 viewMatrix = translate(mat4(1.0f), vec3(-position.x, -position.y, -position.z));
    viewMatrix *= lookAt(
                      vec3(position.x, position.y, position.z),
                      vec3(target.x, target.y, target.z),
                      vec3(0, 1, 0)
                  );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&viewMatrix[0][0]);
}
