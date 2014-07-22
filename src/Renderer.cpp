#include <Renderer.h>
#include <GLheaders.h>
#include <Shader.h>

const char* vs = R"(
    varying vec3 normal;
    varying vec3 lightDir;

    void main()
    {

        vec4 p0 = gl_ModelViewMatrix * gl_Vertex;
        gl_Position = gl_ProjectionMatrix * p0;
        lightDir = normalize(vec3(gl_LightSource[0].position-p0));
        normal = gl_NormalMatrix * gl_Normal;

        // normal = gl_NormalMatrix * gl_Normal;
        // gl_Position = ftransform();

    }
)";

const char* ps = R"(
    varying vec3 normal;
    varying vec3 lightDir;

    void main()
    {
        float intensity;
        vec4 color;
        vec3 n = normalize(normal);
        intensity = dot(lightDir,n);

        if (intensity > 0.95)
            color = vec4(1.0,0.5,0.5,1.0);
        else if (intensity > 0.5)
            color = vec4(0.6,0.3,0.3,1.0);
        else if (intensity > 0.25)
            color = vec4(0.4,0.2,0.2,1.0);
        else
            color = vec4(0.2,0.1,0.1,1.0);
        gl_FragColor = color;

    }
)";


const char* vsPhong = R"(
varying vec3 normal, lightDir, eyeVec;

void main()
{   
    normal = gl_NormalMatrix * gl_Normal;

    vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

    lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
    eyeVec = -vVertex;

    gl_Position = ftransform();     
    gl_TexCoord[0] = gl_MultiTexCoord0;
} 
)";

const char* psPhong = R"(
varying vec3 normal, lightDir, eyeVec;
uniform sampler2D texture1;

void main (void)
{
    vec4 final_color = 
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
    (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
                            
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    
    float lambertTerm = dot(N,L);
    
    if(lambertTerm > 0.0)
    {
        final_color += gl_LightSource[0].diffuse * 
                       gl_FrontMaterial.diffuse * 
                       lambertTerm; 
        
        vec3 E = normalize(eyeVec);
        vec3 R = reflect(-L, N);
        float specular = pow( max(dot(R, E), 0.0), 
                         gl_FrontMaterial.shininess );
        final_color += gl_LightSource[0].specular * 
                       gl_FrontMaterial.specular * 
                       specular;    
    }

    gl_FragColor = final_color;         
    // gl_FragColor = texture2D(texture1, gl_TexCoord[0].st) * final_color;
}
)";

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
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 120);

    if (mesh.material.texture) {
        glEnable(GL_TEXTURE_2D);
        mesh.material.texture->use();
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    // glDisable(GL_LIGHTING);

    // glEnable(GL_BLEND);
    // glDisable(GL_DEPTH_TEST);
    // glBlendFunc(GL_ONE, GL_ONE);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    // static Shader shader(vs, ps);
    static Shader shader(vsPhong, psPhong);
    shader.use();


    glBegin(GL_TRIANGLES);
    for (auto& f : mesh.facets) {
        // glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glTexCoord2d(f.textCoords[0].u, f.textCoords[0].v);
        glNormal3d(mesh.vertices[f.a].normal.x, mesh.vertices[f.a].normal.y, mesh.vertices[f.a].normal.z);
        glVertex3d(mesh.vertices[f.a].position.x, mesh.vertices[f.a].position.y, mesh.vertices[f.a].position.z);

        // glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glTexCoord2d(f.textCoords[1].u, f.textCoords[1].v);
        glNormal3d(mesh.vertices[f.b].normal.x, mesh.vertices[f.b].normal.y, mesh.vertices[f.b].normal.z);
        glVertex3d(mesh.vertices[f.b].position.x, mesh.vertices[f.b].position.y, mesh.vertices[f.b].position.z);

        // glNormal3d(f.normal.x, f.normal.y, f.normal.z);
        glTexCoord2d(f.textCoords[2].u, f.textCoords[2].v);
        glNormal3d(mesh.vertices[f.c].normal.x, mesh.vertices[f.c].normal.y, mesh.vertices[f.c].normal.z);
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
    glShadeModel(GL_FLAT);

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
