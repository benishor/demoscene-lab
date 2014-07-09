#pragma once
#include <chrono>

class Timer {
public:

    Timer();

    double secondsSinceStart();
    double lap();

private:
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> lastLapTime;
};
