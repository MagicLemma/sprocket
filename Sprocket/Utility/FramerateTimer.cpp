#include "FramerateTimer.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Sprocket {

FramerateTimer::FramerateTimer()
    : d_previousTime(glfwGetTime())
    , d_frameCount(0)
{}

void FramerateTimer::update()
{
    double currentTime = glfwGetTime();
    d_frameCount++;
    
    // If a second has passed.
    if (currentTime - d_previousTime >= 1.0) {
        // Display the frame count here any way you want.
        SPKT_LOG_INFO("{}", d_frameCount);
    
        d_frameCount = 0;
        d_previousTime = currentTime;
    }
}

}