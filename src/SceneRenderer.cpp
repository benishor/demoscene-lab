#include <SceneRenderer.h>
#include <LightNode.h>
#include <Shader.h>
#include <Material.h>
#include <MeshNode.h>
#include <DemoData.h>
#include <CameraNode.h>
#include <iostream>

namespace Acidrain {

static const char* vs = R"(
    #version 150 core

    in vec3 position;
    in vec3 normal;
    in vec2 texCoords;

    uniform mat4 mmtx;
    uniform mat4 wlmtx;
    uniform mat4 lpmtx;

    void main() {    
        gl_Position = lpmtx * wlmtx * mmtx * vec4(position, 1);
    }
)";

static const char* ps = R"(
    #version 150 core

    out vec4 colorOut;

    void main (void) {
        colorOut = vec4(1);
    }
)";

static const char* vs2 = R"(
    #version 150 core

    uniform mat4 mmtx;
    uniform mat4 vmtx;
    uniform mat4 pmtx;

    in vec3 position;
    in vec3 normal;

    void main() {    
        gl_Position = pmtx * vmtx * mmtx * vec4(position, 1);
    }
)";

static const char* ps2 = R"(
    #version 150 core
    out vec4 outColor;

    void main() {
        outColor = vec4(1, 1, 1, 1);
    }
)";

SceneRenderer::SceneRenderer() {
    glGenFramebuffers(1, &fboId);

    firstPassShadowShader = std::make_shared<Shader>(vs, ps);

    firstPassShadowMaterial = std::make_shared<Material>();
    firstPassShadowMaterial->shader = firstPassShadowShader;
    firstPassShadowMaterial->zBufferWrite = true;
    firstPassShadowMaterial->zBufferTest  = true;
    firstPassShadowMaterial->transparent  = false;
    firstPassShadowMaterial->cullFaces    = true;
    firstPassShadowMaterial->cullFrontFaces = false;

    secondPassShadowShader = std::make_shared<Shader>(vs2, ps2);

    secondPassShadowMaterial = std::make_shared<Material>();
    secondPassShadowMaterial->shader = secondPassShadowShader;
    secondPassShadowMaterial->diffuse = glm::vec4(1, 0, 0, 1);
    secondPassShadowMaterial->zBufferWrite = true;
    secondPassShadowMaterial->zBufferTest  = true;
    secondPassShadowMaterial->transparent  = false;
    secondPassShadowMaterial->cullFaces    = false;
    secondPassShadowMaterial->flatShaded   = false;
}

void SceneRenderer::attachDepthTexture(Texture& texture) {
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.getId(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void SceneRenderer::render(const SceneTree& scene, const std::string& cameraName) {
    std::shared_ptr<SceneNode> camNode = scene.nodeByNameAndType(cameraName, SceneNodeType::Camera);
    CameraNode& camera = camNode->asCameraNode();

    shaderConstants.set(Uniforms::ViewMatrix, camera.viewMatrix);
    shaderConstants.set(Uniforms::ProjectionMatrix, camera.projectionMatrix);
    shaderConstants.set(Uniforms::NumberOfLights, (int)scene.lights.size());
    // TODO: inject this from outside
    // shaderConstants.set(Uniforms::DemoPartNormalizedTime, normalizedTime);

    setMaterial(firstPassShadowMaterial);
    // draw opaque nodes
    for (auto& node : scene.nodes) {
        if (node->type == SceneNodeType::Mesh) {
            MeshNode& meshNode = node->asMeshNode();

            shaderConstants.set(Uniforms::ModelToWorldMatrix, meshNode.modelToWorldSpaceMatrix);
            shaderConstants.set(Uniforms::MaterialAmbientColor, meshNode.material->ambient);
            shaderConstants.set(Uniforms::MaterialDiffuseColor, meshNode.material->diffuse);
            shaderConstants.set(Uniforms::MaterialSpecularColor, meshNode.material->specular);
            shaderConstants.set(Uniforms::MaterialShininess, meshNode.material->shininess);

            shaderConstants.applyTo(*secondPassShadowMaterial->shader);

            meshNode.mesh->render(secondPassShadowMaterial->flatShaded);
        }
    }

    unsetMaterial(firstPassShadowMaterial);
}

void SceneRenderer::renderShadowMap(const SceneTree& scene, LightNode& light) {
	light.calculateLocalTransform();

	attachDepthTexture(*light.shadowMap[0]);

    glBindFramebuffer(GL_FRAMEBUFFER,fboId);
    glViewport(0, 0, light.shadowMapWidth, light.shadowMapWidth);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glClear(GL_DEPTH_BUFFER_BIT);

    shaderConstants.set(Uniforms::WorldToLightMatrix, light.worldToLightMatrix[0]);
    shaderConstants.set(Uniforms::LightProjectionMatrix, light.lightProjectionMatrix[0]);
    shaderConstants.set(Uniforms::ShadowMatrix, light.finalShadowMapMatrix[0]);
    shaderConstants.set(Uniforms::ShadowBiasMatrix, light.shadowMapBiasMatrix);

    setMaterial(firstPassShadowMaterial);

    // first pass - draw all objects that may cast shadows as seen from light
    for (auto& node : scene.nodes) {
        if (node->type == SceneNodeType::Mesh) {
            MeshNode& meshNode = node->asMeshNode();
            if (!meshNode.material->transparent && meshNode.material->castsShadows) {

                shaderConstants.set(Uniforms::ModelToWorldMatrix, meshNode.modelToWorldSpaceMatrix);
                shaderConstants.applyTo(*firstPassShadowMaterial->shader);

                meshNode.mesh->render(firstPassShadowMaterial->flatShaded);
            }
        }
    }

    unsetMaterial(firstPassShadowMaterial);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK_LEFT);
    glViewport(0, 0, DemoData::WIDTH, DemoData::HEIGHT);
}

} // namespace Acidrain