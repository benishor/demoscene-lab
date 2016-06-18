#pragma once

#include <SceneTree.h>
#include <string>
#include <Material.h> // for opengl dependencies
#include <ShaderConstants.h>

namespace Acidrain {

class Shader;
class Material;

class SceneRenderer {
public:

	SceneRenderer();

	void render(const SceneTree& scene, const std::string& cameraName);

private:
	void attachDepthTexture(Texture& texture);
	void renderShadowMap(const SceneTree& scene, LightNode& light);

	std::shared_ptr<Shader> firstPassShadowShader;
	std::shared_ptr<Material> firstPassShadowMaterial;

	std::shared_ptr<Shader> secondPassShadowShader;
	std::shared_ptr<Material> secondPassShadowMaterial;
	
	ShaderConstants shaderConstants;
	GLuint fboId;
};


} // namespace Acidrain