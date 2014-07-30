#pragma once

#include <AnimationTrack.h>

namespace Acidrain {

class Vec3Track : public AnimationTrack {
public:

    AnimatedValue 	getValue(const AnimatedValue& first, const AnimatedValue& second, float alpha) override;
    void 			controlObject(void* controlledObject, const AnimatedValue& value) override;
};

} // namespace Acidrain
