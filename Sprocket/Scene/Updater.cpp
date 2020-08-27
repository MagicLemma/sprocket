#include "Updater.h"
#include "Maths.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

void UpdateScene(YAML::Node& scene)
{
    if (!scene["Version"]) { scene["Version"] = 0; }
    int version = scene["Version"].as<int>();

    // 0 -> 1: Replace PhysicsComponent with RigidBody3DComponent,
    // BoxCollider3DComponent, SphereCollider3DComponent and
    // CapsuleCollider3DComponent.
    if (version == 0) {
        SPKT_LOG_INFO("Update file to version 1");
        for (auto& entity : scene["Entities"]) {
            if (auto physics = entity["PhysicsComponent"]) {
                auto& rb = entity["RigidBody3DComponent"];
                rb["velocity"] = physics["velocity"];
                rb["gravity"] = physics["gravity"];
                rb["frozen"] = physics["frozen"];
                rb["bounciness"] = physics["bounciness"];
                rb["frictionCoefficient"] = physics["frictionCoefficient"];
                rb["rollingResistance"] = physics["rollingResistance"];

                auto coll = static_cast<Collider>(physics["collider"].as<int>());
                if (coll == Collider::BOX) {
                    auto& box = entity["BoxCollider3DComponent"];
                    box["mass"] = physics["mass"];
                    box["halfExtents"] = physics["halfExtents"];
                } else if (coll == Collider::SPHERE) {
                    auto& sphere = entity["SphereCollider3DComponent"];
                    sphere["mass"] = physics["mass"];
                    sphere["radius"] = physics["radius"];
                } else if (coll == Collider::CAPSULE) {
                    auto& cap = entity["CapsuleCollider3DComponent"];
                    cap["mass"] = physics["mass"];
                    cap["radius"] = physics["radius"];
                    cap["height"] = physics["height"];
                }
            }
            entity.remove("PhysicsComponent");
        }
        ++version;
    }

    scene["Version"] = version;
}
    
}