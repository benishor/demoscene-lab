#pragma once

#include <SceneNode.h>

namespace Acidrain {

// http://en.wikipedia.org/wiki/Shading#Lighting
enum class LightType {
    Directional,
    Point,
    Spot
};

struct LightNode : SceneNode {
    LightNode();

    LightType   lightType;

    glm::vec4   ambient  = glm::vec4(0.2, 0.2, 0.2, 1);
    glm::vec4   diffuse  = glm::vec4(0.5, 0.5, 0.5, 1);
    glm::vec4   specular = glm::vec4(1);

    glm::vec3   attenuation;

    glm::vec3   spotTarget;
    float       spotCutoff;
    float       spotExponent;
};

	
} // namespace Acidrain
