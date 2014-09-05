#include <DemoPartScene.h>
#include <MeshNode.h>
#include <CameraNode.h>
#include <LightNode.h>
#include <ShaderConstants.h>
#include <DemoData.h>

namespace Acidrain {


const char* vs = R"(
    uniform mat4 mmtx;
    uniform mat4 wlmtx;
    uniform mat4 lpmtx;

    void main() {    
        gl_Position = lpmtx * wlmtx * mmtx * gl_Vertex;
    }
)";

const char* ps = R"(
    void main (void) {
        gl_FragColor = vec4(1, 0, 0, 1);
    }
)";

const char* vs2 = R"(
    uniform mat4 mmtx;
    uniform mat4 wlmtx;
    uniform mat4 lpmtx;
    uniform mat4 vmtx;
    uniform mat3 nmtx;
    uniform mat4 pmtx;
    uniform mat4 smtx;
    uniform mat4 sbmtx;

    varying vec3 vEyeSpaceNormal;
    varying vec3 vEyeSpacePosition;
    varying vec4 vShadowCoords;

    void main() {    
        gl_Position    = pmtx * vmtx * mmtx * gl_Vertex;
        gl_FrontColor  = gl_Color;

        vEyeSpacePosition = (vmtx * mmtx * gl_Vertex).xyz;
        vEyeSpaceNormal   = nmtx * gl_Normal.xyz;
        vShadowCoords     = sbmtx * lpmtx * wlmtx * mmtx * gl_Vertex;
    }
)";

const char* ps2 = R"(
    uniform sampler2DShadow shadowMap;
    uniform vec3 lPosEye;

    const vec3 diffuseColor = vec3(1, 1, 1);

    varying vec3 vEyeSpaceNormal;
    varying vec3 vEyeSpacePosition;
    varying vec4 vShadowCoords;

    void main() {
        vec3 L = (lPosEye - vEyeSpacePosition);
        float d = length(L);
        L = normalize(L);

        vec3 normal = normalize(vEyeSpaceNormal);
        float diffuse = max(0.0, dot(normal, L));

        float shadow = shadow2DProj(shadowMap, vShadowCoords).r;
        diffuse = mix(diffuse, diffuse*shadow, 0.5);

        gl_FragColor = vec4(diffuseColor * diffuse, 1);
    }
)";


const int SHADOWMAP_WIDTH  = 512;
const int SHADOWMAP_HEIGHT = 512;

DemoPartScene::DemoPartScene() {

    firstPassShadowShader = std::shared_ptr<Shader>(new Shader(vs, ps));

    firstPassShadowMaterial = std::shared_ptr<Material>(new Material());
    firstPassShadowMaterial->shader = firstPassShadowShader;
    firstPassShadowMaterial->zBufferWrite = true;
    firstPassShadowMaterial->zBufferTest  = true;
    firstPassShadowMaterial->transparent  = false;
    firstPassShadowMaterial->cullFaces    = true;
    firstPassShadowMaterial->cullFrontFaces = false;

    secondPassShadowShader = std::shared_ptr<Shader>(new Shader(vs2, ps2));

    secondPassShadowMaterial = std::shared_ptr<Material>(new Material());
    secondPassShadowMaterial->shader = secondPassShadowShader;
    secondPassShadowMaterial->zBufferWrite = true;
    secondPassShadowMaterial->zBufferTest  = true;
    secondPassShadowMaterial->transparent  = false;
    secondPassShadowMaterial->cullFaces    = false;
    secondPassShadowMaterial->flatShaded   = true;


    // setup the shadowmap texture
    glGenTextures(1, &shadowMapTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexID);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    // set up FBO to get the depth component
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DemoPartScene::~DemoPartScene() {
}

void DemoPartScene::process(float normalizedTime) {

    if (scene->timeline)
        scene->timeline->setAt(normalizedTime);

    if (scene->tree) {
        scene->tree->transform();

        ShaderConstants shaderConstants;

        // decide which light casts shadows ------------------------------------

        std::shared_ptr<SceneNode> lightNode = scene->tree->nodeByNameAndType(lightName, SceneNodeType::Light);
        LightNode& light = lightNode->asLightNode();

        // set the light MV, P and bias matrices

        glm::mat4 worldToLightMatrix = glm::lookAt(light.position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 lightProjectionMatrix = glm::perspective(50.0f, 1.0f, 0.1f, 100.0f);
        glm::mat4 shadowMapBiasMatrix = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));
        glm::mat4 lightProjectionAndBiasMatrix = shadowMapBiasMatrix * lightProjectionMatrix;
        glm::mat4 finalShadowMapMatrix = lightProjectionAndBiasMatrix * worldToLightMatrix;

        glBindFramebuffer(GL_FRAMEBUFFER,fboID);
        glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);

        shaderConstants.add(ShaderConstantType::WorldToLightMatrix, &worldToLightMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::LightProjectionMatrix, &lightProjectionMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::ShadowMatrix, &finalShadowMapMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::ShadowBiasMatrix, &shadowMapBiasMatrix[0][0]);

        setMaterial(firstPassShadowMaterial);

        // first pass - draw all objects that may cast shadows as seen from light
        for (auto& node : scene->tree->nodes) {
            if (node->type == SceneNodeType::Mesh) {
                MeshNode& meshNode = node->asMeshNode();
                if (!meshNode.material->transparent) {

                    shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
                    shaderConstants.set(*firstPassShadowMaterial->shader);

                    meshNode.mesh->render(firstPassShadowMaterial->flatShaded);
                }
            }
        }

        unsetMaterial(firstPassShadowMaterial);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK_LEFT);
        glViewport(0, 0, DemoData::WIDTH, DemoData::HEIGHT);


        // second pass - draw objects
        std::shared_ptr<SceneNode> camNode = scene->tree->nodeByNameAndType(cameraName, SceneNodeType::Camera);
        CameraNode& camera = camNode->asCameraNode();

        shaderConstants.add(ShaderConstantType::ViewMatrix, &camera.viewMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::ProjectionMatrix, &camera.projectionMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::DemoPartNormalizedTime, &normalizedTime);

        glm::vec3 lightPositionInEyeSpace = light.position - camera.position;
        shaderConstants.add(ShaderConstantType::LightPositionInEyeSpace, &lightPositionInEyeSpace[0] );

        setMaterial(secondPassShadowMaterial);

        // draw opaque nodes
        for (auto& node : scene->tree->nodes) {
            if (node->type == SceneNodeType::Mesh) {
                MeshNode& meshNode = node->asMeshNode();

                glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(camera.viewMatrix * meshNode.modelToWorldSpaceMatrix));

                shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
                shaderConstants.add(ShaderConstantType::NormalMatrix, &normalMatrix[0][0]);

                shaderConstants.set(*secondPassShadowMaterial->shader);

                meshNode.mesh->render(secondPassShadowMaterial->flatShaded);
            }
        }
        unsetMaterial(secondPassShadowMaterial);

        // // draw opaque nodes
        // for (auto& node : scene->tree->nodes) {
        //     if (node->type == SceneNodeType::Mesh) {
        //         MeshNode& meshNode = node->asMeshNode();
        //         if (!meshNode.material->transparent) {
        //             setMaterial(meshNode.material);

        //             shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
        //             shaderConstants.set(*meshNode.material->shader);

        //             meshNode.mesh->render();

        //             unsetMaterial(meshNode.material);
        //         }
        //     }
        // }

        // // draw transparent nodes
        // for (auto& node : scene->tree->nodes) {
        //     if (node->type == SceneNodeType::Mesh) {
        //         MeshNode& meshNode = node->asMeshNode();
        //         if (meshNode.material->transparent) {
        //             setMaterial(meshNode.material);

        //             shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
        //             shaderConstants.set(*meshNode.material->shader);

        //             meshNode.mesh->render();

        //             unsetMaterial(meshNode.material);
        //         }
        //     }
        // }
    }
}


} // namespace Acidrain