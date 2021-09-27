#pragma once
#include <memory>

namespace spkt {

struct physics_engine_runtime;

class physics_engine
{
    std::unique_ptr<physics_engine_runtime> d_data;

public:
    physics_engine();
};

}