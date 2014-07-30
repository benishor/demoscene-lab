#pragma once

namespace Acidrain {

struct DemoPart {
    float   startTime;  // absolute time, since demo starts
    float   endTime;    // absolute time, since demo starts

    virtual void process(float normalizedTime) = 0;
};

} // namespace Acidrain

