#pragma once
#include "Types.h"

#include <chrono>

namespace spkt {

class Stopwatch
{
    using Clock = std::chrono::steady_clock;

    Clock d_clock;
    Clock::time_point d_previousTime;
    Clock::time_point d_currentTime;
    Clock::time_point d_lastTimePrinted;
    
    u32 d_frameCount;
    u32 d_frameRate = 0;

public:
    Stopwatch();
    double on_update();
    u32 Framerate() const { return d_frameRate; }
};

}
