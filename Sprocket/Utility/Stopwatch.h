#pragma once
#include "Types.h"

#include <chrono>

namespace Sprocket {

class Stopwatch
{
    using Clock = std::chrono::steady_clock;

    Clock d_clock;
    Clock::time_point d_previousTime;
    Clock::time_point d_currentTime;

    bool d_printFramerate = false;
    Clock::time_point d_lastTimePrinted;
    
    u32 d_frameCount;
    u32 d_frameRate = 0;

public:
    Stopwatch();
    void OnUpdate();
    double DeltaTime() const;

    void PrintFramerate(bool val) { d_printFramerate = val; }
    u32 Framerate() const { return d_frameRate; }
};

}
