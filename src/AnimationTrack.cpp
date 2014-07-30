#include <AnimationTrack.h>

namespace Acidrain {

void AnimationTrack::addKey(Key* key) {
    keys.push_back(std::shared_ptr<Key>(key));
}

void AnimationTrack::addControlledObject(void* controlledObject) {
    controlledObjects.push_back(controlledObject);
}

void AnimationTrack::setAt(float time) {
}

} // namespace Acidrain