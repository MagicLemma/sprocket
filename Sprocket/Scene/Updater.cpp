#include "Updater.h"
#include "Maths.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

void UpdateScene(YAML::Node& scene)
{
    assert(scene["Version"]);
    int version = scene["Version"].as<int>();

    // 0 -> 1: Replace PhysicsComponent with RigidBody3DComponent,
    // BoxCollider3DComponent, SphereCollider3DComponent and
    // CapsuleCollider3DComponent.
    assert(version != 0);

    // 1 -> 2: Move scale from ModelComponent to TransformComponent
    // and make it a vec3
    assert(version != 1);

    scene["Version"] = version;
}
    
}