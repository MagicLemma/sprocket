#pragma once
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
    unsigned int d_frameCount;

    unsigned int d_frameRate = 0.0f;

public:
    Stopwatch();
    void OnUpdate();
    double DeltaTime() const;

    void PrintFramerate(bool val) { d_printFramerate = val; }
    unsigned int Framerate() const { return d_frameRate; }
};

}
