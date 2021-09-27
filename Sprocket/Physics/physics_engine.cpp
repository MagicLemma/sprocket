#include "physics_engine.h"

#include <reactphysics3d/reactphysics3d.h>

#include <memory>

namespace spkt {
namespace {

// rp3d uses this class as a singleton to instantiate all physics worlds and objects.
// This will be lazily initialised and only kept alive while there is at least one
// active physics engine.
static std::weak_ptr<rp3d::PhysicsCommon> s_physics_common;

std::shared_ptr<rp3d::PhysicsCommon> get_physics_common()
{
    if (auto ptr = s_physics_common.lock()) {
        return ptr;
    }

    auto physics_common = std::make_shared<rp3d::PhysicsCommon>();
    s_physics_common = physics_common;
    return physics_common;
}

}

struct physics_engine_runtime
{
     
};
    
}