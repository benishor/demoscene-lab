#pragma once

#include <DemoPart.h>
#include <memory>
#include <string>
#include <Scene.h>
#include <Material.h>
#include <SceneRenderer.h>

namespace Acidrain {

struct DemoPartScene : DemoPart {

    DemoPartScene();
    virtual ~DemoPartScene();

    std::shared_ptr<SceneRenderer> renderer;
    std::shared_ptr<Scene>      scene;
    std::string                 cameraName;
    std::string                 lightName;
    float                       animationStartTime; // normalized time, [0-1]
    float                       animationEndTime;   // normalized time, [0-1]

    GLuint                      fboID;
    GLuint                      shadowMapTexID;
    std::shared_ptr<Shader>     firstPassShadowShader;
    std::shared_ptr<Material>   firstPassShadowMaterial;

    std::shared_ptr<Shader>     secondPassShadowShader;
    std::shared_ptr<Material>   secondPassShadowMaterial;

    void process(float normalizedTime) override;
};


} // namespace Acidrain

