#include <AnimationTrack.h>

namespace Acidrain {

void AnimationTrack::addKey(Key* key) {
    keys.push_back(std::shared_ptr<Key>(key));
}

void AnimationTrack::addControlledObject(void* controlledObject) {
    controlledObjects.push_back(controlledObject);
}

void AnimationTrack::setAt(float time) {
	std::shared_ptr<Key> first, second;

    auto i = std::begin(keys);

    while (i != std::end(keys)) {
        if ((*i)->time >= time) {
            second = *i;
            break;
        } else {
            first = *i;
        }
        ++i;
    }

    float alpha = (time - first->time) / static_cast<float>(second->time - first->time);
    AnimatedValue interpolatedValue = getValue(first->value, second->value, alpha);

    for (auto o : controlledObjects)
    	controlObject(o, interpolatedValue);
}

} // namespace Acidrain