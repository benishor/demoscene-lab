#pragma once

#include <SceneNode.h>
#include <Texture.h>

namespace Acidrain {

// http://en.wikipedia.org/wiki/Shading#Lighting
enum class LightType {
    Directional,
    Point,
    Spot
};

struct LightNode : SceneNode {
    LightNode(LightType lType);

    void calculateLocalTransform() override;

    LightType   lightType;

    glm::vec4   ambient  = glm::vec4(0.2, 0.2, 0.2, 1);
    glm::vec4   diffuse  = glm::vec4(0.5, 0.5, 0.5, 1);
    glm::vec4   specular = glm::vec4(1);

    glm::vec3   attenuation;

    glm::vec3   spotTarget;
    float       spotCutoff;
    float       spotExponent;

    bool        active = true;

    int                         shadowMapWidth = 1024;
    glm::mat4                   lightProjectionMatrix[6];
    glm::mat4                   worldToLightMatrix[6];
    glm::mat4                   shadowMapBiasMatrix;
    glm::mat4                   finalShadowMapMatrix[6];
    std::shared_ptr<Texture>    shadowMap[6];
};

	
} // namespace Acidrain
