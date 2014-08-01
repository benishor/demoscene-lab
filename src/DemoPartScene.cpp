#include <DemoPartScene.h>
#include <MeshNode.h>
#include <CameraNode.h>
#include <LightNode.h>
#include <ShaderConstants.h>

namespace Acidrain {

void DemoPartScene::process(float normalizedTime) {
    if (scene->timeline)
        scene->timeline->setAt(normalizedTime);

    if (scene->tree) {
        scene->tree->transform();

        std::shared_ptr<SceneNode> camNode = scene->tree->nodeByNameAndType(cameraName, SceneNodeType::Camera);
        CameraNode& camera = camNode->asCameraNode();

        ShaderConstants shaderConstants;
        shaderConstants.add(ShaderConstantType::ViewMatrix, &camera.viewMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::ProjectionMatrix, &camera.projectionMatrix[0][0]);
        shaderConstants.add(ShaderConstantType::DemoPartNormalizedTime, &normalizedTime);

        // draw opaque nodes
        for (auto& node : scene->tree->nodes) {
            if (node->type == SceneNodeType::Mesh) {
                MeshNode& meshNode = node->asMeshNode();
                if (!meshNode.material->transparent) {
                    setMaterial(meshNode.material);

                    shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
                    shaderConstants.set(*meshNode.material->shader);

                    meshNode.mesh->render();

                    unsetMaterial(meshNode.material);
                }
            }
        }

        // draw opaque nodes
        for (auto& node : scene->tree->nodes) {
            if (node->type == SceneNodeType::Mesh) {
                MeshNode& meshNode = node->asMeshNode();
                if (meshNode.material->transparent) {
                    setMaterial(meshNode.material);

                    shaderConstants.add(ShaderConstantType::ModelToWorldMatrix, &meshNode.modelToWorldSpaceMatrix[0][0]);
                    shaderConstants.set(*meshNode.material->shader);

                    meshNode.mesh->render();
                    
                    unsetMaterial(meshNode.material);
                }
            }
        }



    }
}


} // namespace Acidrain