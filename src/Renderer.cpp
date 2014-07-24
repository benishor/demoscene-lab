#include <Renderer.h>
#include <GLheaders.h>


void renderWireframe(const Mesh& mesh) {
    mesh.material.use();

    glBegin(GL_LINES);
    for (auto& e : mesh.edges) {
        glVertex3d(mesh.vertices[e.from].position.x, mesh.vertices[e.from].position.y, mesh.vertices[e.from].position.z);
        glVertex3d(mesh.vertices[e.to].position.x, mesh.vertices[e.to].position.y, mesh.vertices[e.to].position.z);
    }
    glEnd();
}

void renderSolid(Mesh& mesh) {
    mesh.material.use();
    mesh.draw();
}

void renderMesh(Mesh& mesh) {
    switch (mesh.material.fillMode) {
        case FillMode::Wireframe:
            renderWireframe(mesh);
            break;
        case FillMode::Solid:
            renderSolid(mesh);
            break;
        default:
            break;
    }
}


void setupLight(const PointLight& light, int lightIndex) {
    glEnable(GL_LIGHT0 + lightIndex);
    glLightfv(GL_LIGHT0 + lightIndex, GL_AMBIENT, &(light.ambient * light.intensity)[0]);
    glLightfv(GL_LIGHT0 + lightIndex, GL_DIFFUSE, &(light.diffuse * light.intensity)[0]);
    glLightfv(GL_LIGHT0 + lightIndex, GL_SPECULAR, &(light.specular * light.intensity)[0]);

    glLightfv(GL_LIGHT0 + lightIndex, GL_POSITION, new GLfloat[4] {
        static_cast<GLfloat>(light.position.x),
        static_cast<GLfloat>(light.position.y),
        static_cast<GLfloat>(light.position.z),
        1.0f
    });
}

void disableLight(int lightIndex) {
    glDisable(GL_LIGHT0 + lightIndex);
}

void Renderer::render(const Scene& scene, Camera& camera) {
    camera.set();

    glEnable(GL_LIGHTING);

    int lightIndex = 0;
    for (auto& pointLight : scene.pointLights)
        setupLight(pointLight, lightIndex++);

    for (auto& mesh : scene.meshes) {
        glPushMatrix();
        glMultMatrixf(&mesh.modelToWorldMatrix[0][0]);
        renderMesh(const_cast<Mesh&>(mesh));
        glPopMatrix();
    }

    for (int i = 0; i < scene.pointLights.size(); i++)
        disableLight(i);

    glDisable(GL_LIGHTING);
}
