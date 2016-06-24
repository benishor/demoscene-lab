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

    uniform mat4 mmtx;
    uniform mat4 wlmtx;
    uniform mat4 lpmtx;

    in vec3 position;
    in vec3 normal;
    in vec2 texCoords;

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
    uniform mat3 nmtx;
    uniform mat4 pmtx;

    uniform mat4 wlmtx[4];
    uniform mat4 lpmtx[4];
    uniform mat4 sbmtx;

    out vec3 vEyeSpaceNormal;
    out vec3 vEyeSpacePosition;
    out vec4 vShadowCoords[4];
    uniform int numLights;

    in vec3 position;
    in vec3 normal;

    void main() {    
        gl_Position = pmtx * vmtx * mmtx * vec4(position, 1);

        vEyeSpacePosition = (vmtx * mmtx * vec4(position, 1)).xyz;
        vEyeSpaceNormal = normalize(nmtx * normal);

        for (int i = 0; i < numLights; i++) {
	        vShadowCoords[i] = sbmtx * lpmtx[i] * wlmtx[i] * mmtx * vec4(position, 1);
        }
    }
)";

    static const char* ps2 = R"(
    #version 150 core
    out vec4 outColor;

    uniform sampler2DShadow shadowMap1;
    uniform sampler2DShadow shadowMap2;
    uniform sampler2DShadow shadowMap3;
    uniform sampler2DShadow shadowMap4;

    uniform vec3 lPosEye[4];
    uniform vec4 matAmbientCol;
    uniform vec4 matDiffuseCol;
    uniform vec4 matSpecularCol;
    uniform int  numLights;

    struct Light {
    	int type;
    	vec3 position;
    	vec3 target;
    	float spotCutoff;
    	float spotExponent;
    	vec3 attenuation;
    	vec4 ambient;
    	vec4 diffuse;
    	vec4 specular;
    };

    uniform Light light[4];

    const vec2 texmapscale = vec2(1.0f/1024.0f, 1.0f/1024.0f);

    in vec3 vEyeSpaceNormal;
    in vec3 vEyeSpacePosition;
    in vec4 vShadowCoords[4];

    float lookup(int lightIndex, vec2 offset) {
        if (lightIndex == 0)
            return textureProj(
                shadowMap1,
                vShadowCoords[lightIndex] + vec4(
                    offset.x * texmapscale.x * vShadowCoords[lightIndex].w,
                    offset.y * texmapscale.y * vShadowCoords[lightIndex].w,
                    -0.02,
                    0.0
                )
            );
        else if (lightIndex == 1)
            return textureProj(
                shadowMap2,
                vShadowCoords[lightIndex] + vec4(
                    offset.x * texmapscale.x * vShadowCoords[lightIndex].w,
                    offset.y * texmapscale.y * vShadowCoords[lightIndex].w,
                    -0.02,
                    0.0
                )
            );
        else if (lightIndex == 2)
            return textureProj(
                shadowMap3,
                vShadowCoords[lightIndex] + vec4(
                    offset.x * texmapscale.x * vShadowCoords[lightIndex].w,
                    offset.y * texmapscale.y * vShadowCoords[lightIndex].w,
                    -0.02,
                    0.0
                )
            );
        else
            return textureProj(
                shadowMap3,
                vShadowCoords[lightIndex] + vec4(
                    offset.x * texmapscale.x * vShadowCoords[lightIndex].w,
                    offset.y * texmapscale.y * vShadowCoords[lightIndex].w,
                    -0.02,
                    0.0
                )
            );
    }

    void main() {
    	vec4 finalColor = matAmbientCol;
        vec3 normal = normalize(vEyeSpaceNormal);

    	for (int i = 0; i < numLights; i++) {
	        vec3 L = (light[i].position - vEyeSpacePosition);
	        float d = length(L);
	        L = normalize(L);

	        float diffuse = max(0.0, dot(normal, L));

            float shadow = 1.0f;
//            if (vShadowCoords[i].w > 1.0) {
                float x,y;
                shadow = 0.0f;
                for (y = -1.5 ; y <=1.5 ; y+=1.0)
                    for (x = -1.5 ; x <=1.5 ; x+=1.0)
                        shadow += lookup(i, vec2(x,y));

                shadow /= 32.0;
//            }
            //diffuse = mix(diffuse, diffuse * shadow, 0.5);
            //float ambient = mix(1.0, shadow, 0.5);

	        //finalColor += matAmbientCol * ambient * light[i].ambient;
            finalColor += matDiffuseCol * diffuse * light[i].diffuse * shadow;
    	}
        outColor = finalColor;
    }
)";

    SceneRenderer::SceneRenderer() {
        glGenFramebuffers(1, &fboId);

        firstPassShadowShader = std::make_shared<Shader>(vs, ps);

        firstPassShadowMaterial = std::make_shared<Material>();
        firstPassShadowMaterial->shader = firstPassShadowShader;
        firstPassShadowMaterial->zBufferWrite = true;
        firstPassShadowMaterial->zBufferTest = true;
        firstPassShadowMaterial->transparent = false;
        firstPassShadowMaterial->cullFaces = true;
        firstPassShadowMaterial->cullFrontFaces = false;

        secondPassShadowShader = std::make_shared<Shader>(vs2, ps2);

        secondPassShadowMaterial = std::make_shared<Material>();
        secondPassShadowMaterial->shader = secondPassShadowShader;
        secondPassShadowMaterial->diffuse = glm::vec4(1, 0, 0, 1);
        secondPassShadowMaterial->zBufferWrite = true;
        secondPassShadowMaterial->zBufferTest = true;
        secondPassShadowMaterial->transparent = false;
        secondPassShadowMaterial->cullFaces = false;
        secondPassShadowMaterial->flatShaded = false;
    }

    void SceneRenderer::attachDepthTexture(Texture& texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.getId(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void SceneRenderer::render(const SceneTree& scene, const std::string& cameraName) {
        for (auto& node : scene.lights) {
            LightNode& light = node->asLightNode();
            glActiveTexture(GL_TEXTURE0);
            renderShadowMap(scene, light);
        }

        std::shared_ptr<SceneNode> camNode = scene.nodeByNameAndType(cameraName, SceneNodeType::Camera);
        CameraNode& camera = camNode->asCameraNode();

        shaderConstants.set(Uniforms::ViewMatrix, camera.viewMatrix);
        shaderConstants.set(Uniforms::ProjectionMatrix, camera.projectionMatrix);
        shaderConstants.set(Uniforms::NumberOfLights, (int) scene.lights.size());
        // TODO: inject this from outside
        // shaderConstants.set(Uniforms::DemoPartNormalizedTime, normalizedTime);

        setMaterial(secondPassShadowMaterial);

        int lightIndex = 0;
        for (auto& node : scene.lights) {
            LightNode& light = node->asLightNode();

            shaderConstants.set(Uniforms::WorldToLightMatrix, light.worldToLightMatrix[0], lightIndex);
            shaderConstants.set(Uniforms::LightProjectionMatrix, light.lightProjectionMatrix[0], lightIndex);
            shaderConstants.set(Uniforms::ShadowMatrix, light.finalShadowMapMatrix[0], lightIndex);
            shaderConstants.set(Uniforms::ShadowBiasMatrix, light.shadowMapBiasMatrix);

            glm::vec3 lightPositionInEyeSpace = light.position - camera.position;
            shaderConstants.set(Uniforms::LightPosition, lightPositionInEyeSpace, lightIndex);

            glm::vec3 lightTargetInEyeSpace = light.spotTarget - camera.position;
            shaderConstants.set(Uniforms::LightTarget, lightTargetInEyeSpace, lightIndex);

            shaderConstants.set(Uniforms::LightAmbient, light.ambient, lightIndex);
            shaderConstants.set(Uniforms::LightDiffuse, light.diffuse, lightIndex);
            shaderConstants.set(Uniforms::LightSpecular, light.specular, lightIndex);

            shaderConstants.set(Uniforms::LightSpotCutoff, light.spotCutoff, lightIndex);
            shaderConstants.set(Uniforms::LightSpotExponent, light.spotExponent, lightIndex);
            shaderConstants.set(Uniforms::LightAttenuation, light.attenuation, lightIndex);

            glActiveTexture(GL_TEXTURE0 + 4 + lightIndex);
            glBindTexture(GL_TEXTURE_2D, light.shadowMap[0]->getId());

            if (lightIndex == 0) {
                shaderConstants.set(Uniforms::ShadowMap1, lightIndex + 4);
            } else if (lightIndex == 1) {
                shaderConstants.set(Uniforms::ShadowMap2, lightIndex + 4);
            } else if (lightIndex == 2) {
                shaderConstants.set(Uniforms::ShadowMap3, lightIndex + 4);
            } else if (lightIndex == 3) {
                shaderConstants.set(Uniforms::ShadowMap4, lightIndex + 4);
            }

            lightIndex++;
        }

        // draw opaque nodes
        for (auto& node : scene.nodes) {
            if (node->type == SceneNodeType::Mesh) {
                MeshNode& meshNode = node->asMeshNode();

                glm::mat3 normalMatrix = glm::inverseTranspose(
                        glm::mat3(camera.viewMatrix * meshNode.modelToWorldSpaceMatrix));

                shaderConstants.set(Uniforms::ModelToWorldMatrix, meshNode.modelToWorldSpaceMatrix);
                shaderConstants.set(Uniforms::NormalMatrix, normalMatrix);
                shaderConstants.set(Uniforms::MaterialAmbientColor, meshNode.material->ambient);
                shaderConstants.set(Uniforms::MaterialDiffuseColor, meshNode.material->diffuse);
                shaderConstants.set(Uniforms::MaterialSpecularColor, meshNode.material->specular);
                shaderConstants.set(Uniforms::MaterialShininess, meshNode.material->shininess);

                shaderConstants.applyTo(*secondPassShadowMaterial->shader);

                meshNode.mesh->render(secondPassShadowMaterial->flatShaded);
            }
        }

        unsetMaterial(secondPassShadowMaterial);

    }

    void SceneRenderer::renderShadowMap(const SceneTree& scene, LightNode& light) {
        light.calculateLocalTransform();

        attachDepthTexture(*light.shadowMap[0]);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
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