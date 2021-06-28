#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

using registry = apx::registry<
    spkt::TemporaryComponent,
    spkt::NameComponent,
    spkt::Transform2DComponent,
    spkt::Transform3DComponent,
    spkt::ModelComponent,
    spkt::RigidBody3DComponent,
    spkt::BoxCollider3DComponent,
    spkt::SphereCollider3DComponent,
    spkt::CapsuleCollider3DComponent,
    spkt::ScriptComponent,
    spkt::Camera3DComponent,
    spkt::SelectComponent,
    spkt::PathComponent,
    spkt::LightComponent,
    spkt::SunComponent,
    spkt::AmbienceComponent,
    spkt::ParticleComponent,
    spkt::MeshAnimationComponent,
    spkt::Singleton,
    spkt::PhysicsSingleton,
    spkt::InputSingleton,
    spkt::GameGridSingleton,
    spkt::TileMapSingleton,
    spkt::CameraSingleton
>;

using entity = typename registry::handle_type;

inline constexpr entity null;

}
