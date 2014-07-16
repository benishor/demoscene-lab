#include <Renderer.h>
#include <GLheaders.h>

void renderWireframe(const Mesh& mesh) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mesh.material.ambient.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesh.material.diffuse.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesh.material.specular.array);
    glBegin(GL_LINES);
    for (auto& e : mesh.edges) {
        glVertex3d(mesh.vertices[e.from].position.x, mesh.vertices[e.from].position.y, mesh.vertices[e.from].position.z);
        glVertex3d(mesh.vertices[e.to].position.x, mesh.vertices[e.to].position.y, mesh.vertices[e.to].position.z);
    }
    glEnd();
}

void renderSolid(const Mesh& mesh) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mesh.material.ambient.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesh.material.diffuse.array);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesh.material.specular.array);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 2);

    glBegin(GL_TRIANGLES);
    for (auto& f : mesh.facets) {
        glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glVertex3d(mesh.vertices[f.a].position.x, mesh.vertices[f.a].position.y, mesh.vertices[f.a].position.z);

        glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glVertex3d(mesh.vertices[f.b].position.x, mesh.vertices[f.b].position.y, mesh.vertices[f.b].position.z);

        glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glVertex3d(mesh.vertices[f.c].position.x, mesh.vertices[f.c].position.y, mesh.vertices[f.c].position.z);
    }
    glEnd();
}

void renderMesh(const Mesh& mesh) {
    switch (mesh.material.renderType) {
        case RenderType::Wireframe:
            renderWireframe(mesh);
            break;
        case RenderType::Solid:
            renderSolid(mesh);
            break;
        default:
            break;
    }
}


void setupLight(const PointLight& light, int lightIndex) {
    glEnable(GL_LIGHT0 + lightIndex);
    glLightfv(GL_LIGHT0 + lightIndex, GL_AMBIENT, (light.ambient * light.intensity).array);
    glLightfv(GL_LIGHT0 + lightIndex, GL_DIFFUSE, (light.diffuse * light.intensity).array);
    glLightfv(GL_LIGHT0 + lightIndex, GL_SPECULAR, (light.specular * light.intensity).array);

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

void applyTransformation(const Mesh& mesh) {
    glTranslatef(mesh.position.x, mesh.position.y, mesh.position.z);
    glRotated(mesh.rotationAngle, mesh.rotationAxis.x, mesh.rotationAxis.y, mesh.rotationAxis.z);
}


void Renderer::render(const Scene& scene) {
    // it may be smooth but we use face normals, so we'll get flat shading
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    int lightIndex = 0;
    for (auto& pointLight : scene.pointLights)
        setupLight(pointLight, lightIndex++);

    for (auto& mesh : scene.meshes) {
        glPushMatrix();
        applyTransformation(mesh);
        renderMesh(mesh);
        glPopMatrix();
    }

    for (int i = 0; i < scene.pointLights.size(); i++)
        disableLight(i);

    glDisable(GL_LIGHTING);
}
