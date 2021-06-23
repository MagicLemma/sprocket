#pragma once
#include "Components.h"

#include "apecs.hpp"

namespace spkt {

// TODO: Allow for plugins to access the entire spec, so that we can have a
// {{Comp::Util.comma}} plugin which does not put the comma on the end. We currently
// have to remove this manually after generating. If the plugin could see the whole spec
// it would know if the current component is the last one.
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
    Sprocket::GridComponent,
    Sprocket::LightComponent,
    Sprocket::SunComponent,
    Sprocket::AmbienceComponent,
    Sprocket::ParticleComponent,
    Sprocket::MeshAnimationComponent,
    Sprocket::Singleton,
    Sprocket::CollisionSingleton
>;

using entity = typename registry::handle_type;
using identifier = apx::entity;

inline constexpr entity null;

template <typename Comp>
struct added
{
    spkt::entity entity;
    added(spkt::entity e) : entity(e) {}
};

template <typename Comp>
struct removed
{
    spkt::entity entity;
    removed(spkt::entity e) : entity(e) {}
};

}
