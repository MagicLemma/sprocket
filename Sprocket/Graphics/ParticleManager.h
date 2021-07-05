#pragma once
#include "Maths.h"
#include "Buffer.h"

#include "apecs.hpp"

#include <memory>
#include <array>
#include <vector>

namespace spkt {

class ParticleManager
{
    std::unique_ptr<Buffer> d_instances;

public:
    ParticleManager();

    void on_update(apx::registry& registry, double dt);

    Buffer* GetInstances() const { return d_instances.get(); }

};

}