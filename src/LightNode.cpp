#include <LightNode.h>

namespace Acidrain {

LightNode::LightNode(LightType lType) {
    type 		= SceneNodeType::Light;
    lightType 	= lType;

    shadowMapBiasMatrix = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(0.5, 0.5, 0.5));

	int texturesToGenerate = lightType == LightType::Point ? 6 : 1;
	for (int i = 0; i < texturesToGenerate; i++) {
		GLuint shadowMapTexID;

	    // setup the shadowmap texture
	    glGenTextures(1, &shadowMapTexID);
	    glBindTexture(GL_TEXTURE_2D, shadowMapTexID);

	    // set texture parameters
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
	    	shadowMapWidth, shadowMapWidth, 
	    	0, 
	    	GL_DEPTH_COMPONENT, 
	    	GL_UNSIGNED_BYTE, 
	    	NULL);

	    shadowMap[i] = std::make_shared<Texture>(shadowMapTexID, shadowMapWidth, shadowMapWidth);
	}
}

void LightNode::calculateLocalTransform() {
	if (lightType == LightType::Spot) {
	    worldToLightMatrix[0] = glm::lookAt(position, spotTarget, glm::vec3(0, 1, 0));
	    lightProjectionMatrix[0] = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
	    glm::mat4 lightProjectionAndBiasMatrix = shadowMapBiasMatrix * lightProjectionMatrix[0];
	    finalShadowMapMatrix[0] = lightProjectionAndBiasMatrix * worldToLightMatrix[0];
	}
}

} // namespace Acidrain