#include <Vec3Track.h>
#include <GLMheaders.h>

namespace Acidrain {

AnimatedValue Vec3Track::getValue(const AnimatedValue& first, const AnimatedValue& second, float alpha) {
    AnimatedValue result;
    result.vec3Val = glm::mix(first.vec3Val, second.vec3Val, alpha);
    return result;
};

void Vec3Track::controlObject(void* controlledObject, const AnimatedValue& value) {
    *reinterpret_cast<glm::vec3*>(controlledObject) = value.vec3Val;
}

} // namespace Acidrain