#include <Timeline.h>

namespace Acidrain {

void Timeline::setAt(float time) {
    for (auto& track : tracks)
        track->setAt(time);
}

} // namespace Acidrain

