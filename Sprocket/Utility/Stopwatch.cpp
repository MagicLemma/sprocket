#include "Stopwatch.h"
#include "Log.h"

namespace Sprocket {

Stopwatch::Stopwatch()
    : d_clock()
    , d_previousTime(d_clock.now())
    , d_currentTime(d_previousTime)
    , d_lastTimePrinted(d_previousTime)
    , d_frameCount(0)
{}

void Stopwatch::OnUpdate()
{
    d_previousTime = d_currentTime;
    d_currentTime = d_clock.now();
    
    ++d_frameCount;

    if (d_currentTime - d_lastTimePrinted >= std::chrono::seconds(1)) {
        if (d_printFramerate) { SPKT_LOG_INFO("{}", d_frameCount); }
        d_frameCount = 0;
        d_lastTimePrinted = d_currentTime;
    }
}

double Stopwatch::DeltaTime() const
{
    std::chrono::duration<double> dt = d_currentTime - d_previousTime;
    return dt.count();
}

}