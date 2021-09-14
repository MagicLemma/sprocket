#pragma once
#include <chrono>
#include <cstddef>

namespace spkt {

class timer
{
    using clock = std::chrono::steady_clock;

    clock d_clock;
    clock::time_point d_prev_time;
    clock::time_point d_curr_time;
    clock::time_point d_last_time_printed;
    
    std::uint32_t d_frame_count;
    std::uint32_t d_frame_rate = 0;

public:
    timer();
    double on_update();
    std::uint32_t frame_rate() const { return d_frame_rate; }
};

}
