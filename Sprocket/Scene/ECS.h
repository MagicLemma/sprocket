#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

using registry = apx::registry<
    Sprocket::TemporaryComponent,
    Sprocket::NameComponent,
    Sprocket::Transform2DComponent,
    Sprocket::Transform3DComponent,
    Sprocket::ModelComponent,
    Sprocket::RigidBody3DComponent,
    Sprocket::BoxCollider3DComponent,
    Sprocket::SphereCollider3DComponent,
    Sprocket::CapsuleCollider3DComponent,
    Sprocket::ScriptComponent,
    Sprocket::Camera3DComponent,
    Sprocket::SelectComponent,
    Sprocket::PathComponent,
    Sprocket::LightComponent,
    Sprocket::SunComponent,
    Sprocket::AmbienceComponent,
    Sprocket::ParticleComponent,
    Sprocket::MeshAnimationComponent,
    Sprocket::Singleton,
    Sprocket::PhysicsSingleton,
    Sprocket::InputSingleton,
    Sprocket::GameGridSingleton,
    Sprocket::TileMapSingleton,
    Sprocket::CameraSingleton
>;

using entity = typename registry::handle_type;

inline constexpr entity null;

}
