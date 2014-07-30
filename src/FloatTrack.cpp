#include <FloatTrack.h>
#include <GLMheaders.h>

namespace Acidrain {

AnimatedValue FloatTrack::getValue(const AnimatedValue& first, const AnimatedValue& second, float alpha) {
    AnimatedValue result;
    result.floatVal = glm::mix(first.floatVal, second.floatVal, alpha);
    return result;
};

void FloatTrack::controlObject(void* controlledObject, const AnimatedValue& value) {
    *reinterpret_cast<float*>(controlledObject) = value.floatVal;
}

} // namespace Acidrain