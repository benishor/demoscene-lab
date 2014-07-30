#include <LightNode.h>

namespace Acidrain {

LightNode::LightNode() {
    type 		= SceneNodeType::Light;
    lightType 	= LightType::Directional;
}

} // namespace Acidrain