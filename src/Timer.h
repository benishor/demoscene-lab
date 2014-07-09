#pragma once
#include <chrono>

class Timer {
public:

    Timer();

    double secondsSinceStart();

private:
    std::chrono::time_point<std::chrono::system_clock> startTime;
};
