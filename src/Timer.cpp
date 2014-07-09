#include <Timer.h>

Timer::Timer() {
    startTime = std::chrono::system_clock::now();
}

double Timer::secondsSinceStart() {
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
    return elapsedSeconds.count();
}
