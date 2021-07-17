#pragma once
#include <chrono>
#include <cstddef>

namespace spkt {

class Stopwatch
{
    using Clock = std::chrono::steady_clock;

    Clock d_clock;
    Clock::time_point d_previousTime;
    Clock::time_point d_currentTime;
    Clock::time_point d_lastTimePrinted;
    
    std::uint32_t d_frameCount;
    std::uint32_t d_frameRate = 0;

public:
    Stopwatch();
    double on_update();
    std::uint32_t Framerate() const { return d_frameRate; }
};

}
