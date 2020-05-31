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

public:
    Stopwatch();
    void OnUpdate();
    float DeltaTime() const;

    void PrintFramerate(bool val) { d_printFramerate = val; }
};

}
