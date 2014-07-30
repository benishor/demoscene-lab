#pragma once

#include <AnimationTrack.h>

namespace Acidrain {

struct Timeline {
    void setAt(float time);
    std::vector<std::shared_ptr<AnimationTrack>> tracks;
};

} // namespace Acidrain

