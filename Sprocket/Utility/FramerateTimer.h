#pragma once

namespace Sprocket {

class FramerateTimer
// A timer to be created before the game loop and updated every frame.
// This will print the framerate every second.
{
    double d_previousTime;
    int    d_frameCount;

public:
    FramerateTimer();
        // Begin the timer.

    void update();
        // Updates the timer. If it has been longer than a second since
        // it last logged the framerate, it will do so.
};

}
