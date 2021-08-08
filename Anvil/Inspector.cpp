#include "Inspector.h"

#include <Anvil/Anvil.h>

#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Utility/Maths.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/UI/DevUI.h>
#include <Sprocket/Scene/Loader.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void Inspector::Show(Anvil& editor)
{
    spkt::registry& registry = editor.active_scene()->registry;
    spkt::entity entity = editor.selected();

    if (!registry.valid(entity)) {
        if (ImGui::Button("New Entity")) {
            entity = registry.create();
            editor.set_selected(entity);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity);

    if (registry.has<spkt::Runtime>(entity)) {
        auto& c = registry.get<spkt::Runtime>(entity);
        if (ImGui::CollapsingHeader("Runtime")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::Runtime>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::Singleton>(entity)) {
        auto& c = registry.get<spkt::Singleton>(entity);
        if (ImGui::CollapsingHeader("Singleton")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::Singleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::Event>(entity)) {
        auto& c = registry.get<spkt::Event>(entity);
        if (ImGui::CollapsingHeader("Event")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::Event>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::NameComponent>(entity)) {
        auto& c = registry.get<spkt::NameComponent>(entity);
        if (ImGui::CollapsingHeader("Name")) {
            ImGui::PushID(count++);
            spkt::ImGuiXtra::TextModifiable(c.name);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::NameComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::Transform2DComponent>(entity)) {
        auto& c = registry.get<spkt::Transform2DComponent>(entity);
        if (ImGui::CollapsingHeader("Transform 2D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat2("Position", &c.position.x, 0.1f);
            ImGui::DragFloat("Rotation", &c.rotation, 0.01f);
            ImGui::DragFloat2("Scale", &c.scale.x, 0.1f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::Transform2DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::Transform3DComponent>(entity)) {
        auto& c = registry.get<spkt::Transform3DComponent>(entity);
        if (ImGui::CollapsingHeader("Transform 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            spkt::ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
            if (ImGui::Button("Delete")) { registry.remove<spkt::Transform3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::StaticModelComponent>(entity)) {
        auto& c = registry.get<spkt::StaticModelComponent>(entity);
        if (ImGui::CollapsingHeader("Static Model")) {
            ImGui::PushID(count++);
            spkt::ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
            spkt::ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::StaticModelComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::AnimatedModelComponent>(entity)) {
        auto& c = registry.get<spkt::AnimatedModelComponent>(entity);
        if (ImGui::CollapsingHeader("Animated Model")) {
            ImGui::PushID(count++);
            spkt::ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
            spkt::ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
            spkt::ImGuiXtra::TextModifiable(c.animation_name);
            ImGui::DragFloat("Animation Time", &c.animation_time, 0.01f);
            ImGui::DragFloat("Animation Speed", &c.animation_speed, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::AnimatedModelComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::RigidBody3DComponent>(entity)) {
        auto& c = registry.get<spkt::RigidBody3DComponent>(entity);
        if (ImGui::CollapsingHeader("Rigid Body 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::Checkbox("Gravity", &c.gravity);
            ImGui::Checkbox("Frozen", &c.frozen);
            ImGui::SliderFloat("Bounciness", &c.bounciness, 0.0f, 1.0f);
            ImGui::SliderFloat("Friction Coefficient", &c.frictionCoefficient, 0.0f, 1.0f);
            ImGui::SliderFloat("Rolling Resistance", &c.rollingResistance, 0.0f, 1.0f);
            ImGui::DragFloat3("Force", &c.force.x, 0.1f);
            ImGui::Checkbox("OnFloor", &c.onFloor);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::RigidBody3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::BoxCollider3DComponent>(entity)) {
        auto& c = registry.get<spkt::BoxCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Box Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
            ImGui::Checkbox("Apply Scale", &c.applyScale);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::BoxCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::SphereCollider3DComponent>(entity)) {
        auto& c = registry.get<spkt::SphereCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Sphere Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::SphereCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::CapsuleCollider3DComponent>(entity)) {
        auto& c = registry.get<spkt::CapsuleCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Capsule Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::CapsuleCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::ScriptComponent>(entity)) {
        auto& c = registry.get<spkt::ScriptComponent>(entity);
        if (ImGui::CollapsingHeader("Script")) {
            ImGui::PushID(count++);
            spkt::ImGuiXtra::File("Script", editor.window(), &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::ScriptComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::Camera3DComponent>(entity)) {
        auto& c = registry.get<spkt::Camera3DComponent>(entity);
        if (ImGui::CollapsingHeader("Camera 3D")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("FOV", &c.fov, 0.01f);
            ImGui::DragFloat("Pitch", &c.pitch, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::Camera3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::PathComponent>(entity)) {
        auto& c = registry.get<spkt::PathComponent>(entity);
        if (ImGui::CollapsingHeader("Path")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::PathComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::LightComponent>(entity)) {
        auto& c = registry.get<spkt::LightComponent>(entity);
        if (ImGui::CollapsingHeader("Light")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::LightComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::SunComponent>(entity)) {
        auto& c = registry.get<spkt::SunComponent>(entity);
        if (ImGui::CollapsingHeader("Sun")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            ImGui::DragFloat3("Direction", &c.direction.x, 0.1f);
            ImGui::Checkbox("Shadows", &c.shadows);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::SunComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::AmbienceComponent>(entity)) {
        auto& c = registry.get<spkt::AmbienceComponent>(entity);
        if (ImGui::CollapsingHeader("Ambience")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::AmbienceComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::ParticleComponent>(entity)) {
        auto& c = registry.get<spkt::ParticleComponent>(entity);
        if (ImGui::CollapsingHeader("Particle")) {
            ImGui::PushID(count++);
            ImGui::DragFloat("Interval", &c.interval, 0.01f);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.01f);
            ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGui::DragFloat("Life", &c.life, 0.01f);
            ImGui::DragFloat("Accumulator", &c.accumulator, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::ParticleComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::CollisionEvent>(entity)) {
        auto& c = registry.get<spkt::CollisionEvent>(entity);
        if (ImGui::CollapsingHeader("Collision Event")) {
            ImGui::PushID(count++);
            ;
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::CollisionEvent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::PhysicsSingleton>(entity)) {
        auto& c = registry.get<spkt::PhysicsSingleton>(entity);
        if (ImGui::CollapsingHeader("Physics Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::PhysicsSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::InputSingleton>(entity)) {
        auto& c = registry.get<spkt::InputSingleton>(entity);
        if (ImGui::CollapsingHeader("Input Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            ;
            ;
            ImGui::DragFloat2("Mouse Position", &c.mouse_pos.x, 0.1f);
            ImGui::DragFloat2("Mouse Position Last Frame", &c.mouse_pos_last_frame.x, 0.1f);
            ImGui::DragFloat2("Mouse Offset", &c.mouse_offset.x, 0.1f);
            ImGui::DragFloat2("Mouse Scrolled", &c.mouse_scrolled.x, 0.1f);
            ImGui::DragFloat("Window Width", &c.window_width, 0.01f);
            ImGui::DragFloat("Window Height", &c.window_height, 0.01f);
            ImGui::Checkbox("Window Resized", &c.window_resized);
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::InputSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::GameGridSingleton>(entity)) {
        auto& c = registry.get<spkt::GameGridSingleton>(entity);
        if (ImGui::CollapsingHeader("Game Grid Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            ;
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::GameGridSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::TileMapSingleton>(entity)) {
        auto& c = registry.get<spkt::TileMapSingleton>(entity);
        if (ImGui::CollapsingHeader("Tile Map Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::TileMapSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::CameraSingleton>(entity)) {
        auto& c = registry.get<spkt::CameraSingleton>(entity);
        if (ImGui::CollapsingHeader("Camera Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::CameraSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<spkt::ParticleSingleton>(entity)) {
        auto& c = registry.get<spkt::ParticleSingleton>(entity);
        if (ImGui::CollapsingHeader("Particle Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<spkt::ParticleSingleton>(entity); }
            ImGui::PopID();
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        if (!registry.has<spkt::Runtime>(entity) && ImGui::Selectable("Runtime")) {
            spkt::Runtime c;
            registry.add<spkt::Runtime>(entity, c);
        }
        if (!registry.has<spkt::Singleton>(entity) && ImGui::Selectable("Singleton")) {
            spkt::Singleton c;
            registry.add<spkt::Singleton>(entity, c);
        }
        if (!registry.has<spkt::Event>(entity) && ImGui::Selectable("Event")) {
            spkt::Event c;
            registry.add<spkt::Event>(entity, c);
        }
        if (!registry.has<spkt::NameComponent>(entity) && ImGui::Selectable("Name")) {
            spkt::NameComponent c;
            registry.add<spkt::NameComponent>(entity, c);
        }
        if (!registry.has<spkt::Transform2DComponent>(entity) && ImGui::Selectable("Transform 2D")) {
            spkt::Transform2DComponent c;
            registry.add<spkt::Transform2DComponent>(entity, c);
        }
        if (!registry.has<spkt::Transform3DComponent>(entity) && ImGui::Selectable("Transform 3D")) {
            spkt::Transform3DComponent c;
            registry.add<spkt::Transform3DComponent>(entity, c);
        }
        if (!registry.has<spkt::StaticModelComponent>(entity) && ImGui::Selectable("Static Model")) {
            spkt::StaticModelComponent c;
            registry.add<spkt::StaticModelComponent>(entity, c);
        }
        if (!registry.has<spkt::AnimatedModelComponent>(entity) && ImGui::Selectable("Animated Model")) {
            spkt::AnimatedModelComponent c;
            registry.add<spkt::AnimatedModelComponent>(entity, c);
        }
        if (!registry.has<spkt::RigidBody3DComponent>(entity) && ImGui::Selectable("Rigid Body 3D")) {
            spkt::RigidBody3DComponent c;
            registry.add<spkt::RigidBody3DComponent>(entity, c);
        }
        if (!registry.has<spkt::BoxCollider3DComponent>(entity) && ImGui::Selectable("Box Collider 3D")) {
            spkt::BoxCollider3DComponent c;
            registry.add<spkt::BoxCollider3DComponent>(entity, c);
        }
        if (!registry.has<spkt::SphereCollider3DComponent>(entity) && ImGui::Selectable("Sphere Collider 3D")) {
            spkt::SphereCollider3DComponent c;
            registry.add<spkt::SphereCollider3DComponent>(entity, c);
        }
        if (!registry.has<spkt::CapsuleCollider3DComponent>(entity) && ImGui::Selectable("Capsule Collider 3D")) {
            spkt::CapsuleCollider3DComponent c;
            registry.add<spkt::CapsuleCollider3DComponent>(entity, c);
        }
        if (!registry.has<spkt::ScriptComponent>(entity) && ImGui::Selectable("Script")) {
            spkt::ScriptComponent c;
            registry.add<spkt::ScriptComponent>(entity, c);
        }
        if (!registry.has<spkt::Camera3DComponent>(entity) && ImGui::Selectable("Camera 3D")) {
            spkt::Camera3DComponent c;
            registry.add<spkt::Camera3DComponent>(entity, c);
        }
        if (!registry.has<spkt::PathComponent>(entity) && ImGui::Selectable("Path")) {
            spkt::PathComponent c;
            registry.add<spkt::PathComponent>(entity, c);
        }
        if (!registry.has<spkt::LightComponent>(entity) && ImGui::Selectable("Light")) {
            spkt::LightComponent c;
            registry.add<spkt::LightComponent>(entity, c);
        }
        if (!registry.has<spkt::SunComponent>(entity) && ImGui::Selectable("Sun")) {
            spkt::SunComponent c;
            registry.add<spkt::SunComponent>(entity, c);
        }
        if (!registry.has<spkt::AmbienceComponent>(entity) && ImGui::Selectable("Ambience")) {
            spkt::AmbienceComponent c;
            registry.add<spkt::AmbienceComponent>(entity, c);
        }
        if (!registry.has<spkt::ParticleComponent>(entity) && ImGui::Selectable("Particle")) {
            spkt::ParticleComponent c;
            registry.add<spkt::ParticleComponent>(entity, c);
        }
        if (!registry.has<spkt::CollisionEvent>(entity) && ImGui::Selectable("Collision Event")) {
            spkt::CollisionEvent c;
            registry.add<spkt::CollisionEvent>(entity, c);
        }
        if (!registry.has<spkt::PhysicsSingleton>(entity) && ImGui::Selectable("Physics Singleton")) {
            spkt::PhysicsSingleton c;
            registry.add<spkt::PhysicsSingleton>(entity, c);
        }
        if (!registry.has<spkt::InputSingleton>(entity) && ImGui::Selectable("Input Singleton")) {
            spkt::InputSingleton c;
            registry.add<spkt::InputSingleton>(entity, c);
        }
        if (!registry.has<spkt::GameGridSingleton>(entity) && ImGui::Selectable("Game Grid Singleton")) {
            spkt::GameGridSingleton c;
            registry.add<spkt::GameGridSingleton>(entity, c);
        }
        if (!registry.has<spkt::TileMapSingleton>(entity) && ImGui::Selectable("Tile Map Singleton")) {
            spkt::TileMapSingleton c;
            registry.add<spkt::TileMapSingleton>(entity, c);
        }
        if (!registry.has<spkt::CameraSingleton>(entity) && ImGui::Selectable("Camera Singleton")) {
            spkt::CameraSingleton c;
            registry.add<spkt::CameraSingleton>(entity, c);
        }
        if (!registry.has<spkt::ParticleSingleton>(entity) && ImGui::Selectable("Particle Singleton")) {
            spkt::ParticleSingleton c;
            registry.add<spkt::ParticleSingleton>(entity, c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = spkt::copy_entity(editor.active_scene()->registry, entity);
        editor.set_selected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        registry.destroy(entity);
        editor.clear_selected();
    }
}
