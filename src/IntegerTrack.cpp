#include <IntegerTrack.h>

namespace Acidrain {

AnimatedValue IntegerTrack::getValue(const AnimatedValue& first, const AnimatedValue& second, float alpha) {
    // no interpolation here
    AnimatedValue result;
    result.intVal = static_cast<int>(first.intVal + (second.intVal - first.intVal) * alpha);
    return result;
};

void IntegerTrack::controlObject(void* controlledObject, const AnimatedValue& value) {
    *reinterpret_cast<int*>(controlledObject) = value.intVal;
}

} // namespace Acidrain

