#include "Inspector.h"
#include "Anvil.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace spkt {

void Inspector::Show(Anvil& editor)
{
    apx::registry& registry = editor.active_scene()->Entities();
    apx::entity entity = editor.selected();

    if (!registry.valid(entity)) {
        if (ImGui::Button("New Entity")) {
            entity = registry.create();
            editor.set_selected(entity);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity);

    if (registry.has<Runtime>(entity)) {
        auto& c = registry.get<Runtime>(entity);
        if (ImGui::CollapsingHeader("Runtime")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<Runtime>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<Singleton>(entity)) {
        auto& c = registry.get<Singleton>(entity);
        if (ImGui::CollapsingHeader("Singleton")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<Singleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<Event>(entity)) {
        auto& c = registry.get<Event>(entity);
        if (ImGui::CollapsingHeader("Event")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { registry.remove<Event>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<NameComponent>(entity)) {
        auto& c = registry.get<NameComponent>(entity);
        if (ImGui::CollapsingHeader("Name")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            
            if (ImGui::Button("Delete")) { registry.remove<NameComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<Transform2DComponent>(entity)) {
        auto& c = registry.get<Transform2DComponent>(entity);
        if (ImGui::CollapsingHeader("Transform 2D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat2("Position", &c.position.x, 0.1f);
            ImGui::DragFloat("Rotation", &c.rotation, 0.01f);
            ImGui::DragFloat2("Scale", &c.scale.x, 0.1f);
            
            if (ImGui::Button("Delete")) { registry.remove<Transform2DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<Transform3DComponent>(entity)) {
        auto& c = registry.get<Transform3DComponent>(entity);
        if (ImGui::CollapsingHeader("Transform 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
            if (ImGui::Button("Delete")) { registry.remove<Transform3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<ModelComponent>(entity)) {
        auto& c = registry.get<ModelComponent>(entity);
        if (ImGui::CollapsingHeader("Model")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
            ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
            
            if (ImGui::Button("Delete")) { registry.remove<ModelComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<RigidBody3DComponent>(entity)) {
        auto& c = registry.get<RigidBody3DComponent>(entity);
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
            
            if (ImGui::Button("Delete")) { registry.remove<RigidBody3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<BoxCollider3DComponent>(entity)) {
        auto& c = registry.get<BoxCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Box Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
            ImGui::Checkbox("Apply Scale", &c.applyScale);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<BoxCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<SphereCollider3DComponent>(entity)) {
        auto& c = registry.get<SphereCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Sphere Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<SphereCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<CapsuleCollider3DComponent>(entity)) {
        auto& c = registry.get<CapsuleCollider3DComponent>(entity);
        if (ImGui::CollapsingHeader("Capsule Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<CapsuleCollider3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<ScriptComponent>(entity)) {
        auto& c = registry.get<ScriptComponent>(entity);
        if (ImGui::CollapsingHeader("Script")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Script", editor.window(), &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<ScriptComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<Camera3DComponent>(entity)) {
        auto& c = registry.get<Camera3DComponent>(entity);
        if (ImGui::CollapsingHeader("Camera 3D")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("FOV", &c.fov, 0.01f);
            ImGui::DragFloat("Pitch", &c.pitch, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<Camera3DComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<PathComponent>(entity)) {
        auto& c = registry.get<PathComponent>(entity);
        if (ImGui::CollapsingHeader("Path")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<PathComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<LightComponent>(entity)) {
        auto& c = registry.get<LightComponent>(entity);
        if (ImGui::CollapsingHeader("Light")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<LightComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<SunComponent>(entity)) {
        auto& c = registry.get<SunComponent>(entity);
        if (ImGui::CollapsingHeader("Sun")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            ImGui::DragFloat3("Direction", &c.direction.x, 0.1f);
            ImGui::Checkbox("Shadows", &c.shadows);
            
            if (ImGui::Button("Delete")) { registry.remove<SunComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<AmbienceComponent>(entity)) {
        auto& c = registry.get<AmbienceComponent>(entity);
        if (ImGui::CollapsingHeader("Ambience")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<AmbienceComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<ParticleComponent>(entity)) {
        auto& c = registry.get<ParticleComponent>(entity);
        if (ImGui::CollapsingHeader("Particle")) {
            ImGui::PushID(count++);
            ImGui::DragFloat("Interval", &c.interval, 0.01f);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.01f);
            ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGui::DragFloat("Life", &c.life, 0.01f);
            ImGui::DragFloat("Accumulator", &c.accumulator, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<ParticleComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<MeshAnimationComponent>(entity)) {
        auto& c = registry.get<MeshAnimationComponent>(entity);
        if (ImGui::CollapsingHeader("Mesh Animation")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            ImGui::DragFloat("Time", &c.time, 0.01f);
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { registry.remove<MeshAnimationComponent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<CollisionEvent>(entity)) {
        auto& c = registry.get<CollisionEvent>(entity);
        if (ImGui::CollapsingHeader("Collision Event")) {
            ImGui::PushID(count++);
            ;
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<CollisionEvent>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<PhysicsSingleton>(entity)) {
        auto& c = registry.get<PhysicsSingleton>(entity);
        if (ImGui::CollapsingHeader("Physics Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<PhysicsSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<InputSingleton>(entity)) {
        auto& c = registry.get<InputSingleton>(entity);
        if (ImGui::CollapsingHeader("Input Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            ;
            ;
            ImGui::DragFloat2("Mouse Position", &c.mouse_pos.x, 0.1f);
            ImGui::DragFloat2("Mouse Offset", &c.mouse_offset.x, 0.1f);
            ImGui::DragFloat2("Mouse Scrolled", &c.mouse_scrolled.x, 0.1f);
            ImGui::DragFloat("Window Width", &c.window_width, 0.01f);
            ImGui::DragFloat("Window Height", &c.window_height, 0.01f);
            ImGui::Checkbox("Window Resized", &c.window_resized);
            
            if (ImGui::Button("Delete")) { registry.remove<InputSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<GameGridSingleton>(entity)) {
        auto& c = registry.get<GameGridSingleton>(entity);
        if (ImGui::CollapsingHeader("Game Grid Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            ;
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<GameGridSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<TileMapSingleton>(entity)) {
        auto& c = registry.get<TileMapSingleton>(entity);
        if (ImGui::CollapsingHeader("Tile Map Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<TileMapSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<CameraSingleton>(entity)) {
        auto& c = registry.get<CameraSingleton>(entity);
        if (ImGui::CollapsingHeader("Camera Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<CameraSingleton>(entity); }
            ImGui::PopID();
        }
    }

    if (registry.has<ParticleSingleton>(entity)) {
        auto& c = registry.get<ParticleSingleton>(entity);
        if (ImGui::CollapsingHeader("Particle Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { registry.remove<ParticleSingleton>(entity); }
            ImGui::PopID();
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        if (!registry.has<Runtime>(entity) && ImGui::Selectable("Runtime")) {
            Runtime c;
            registry.add<Runtime>(entity, c);
        }
        if (!registry.has<Singleton>(entity) && ImGui::Selectable("Singleton")) {
            Singleton c;
            registry.add<Singleton>(entity, c);
        }
        if (!registry.has<Event>(entity) && ImGui::Selectable("Event")) {
            Event c;
            registry.add<Event>(entity, c);
        }
        if (!registry.has<NameComponent>(entity) && ImGui::Selectable("Name")) {
            NameComponent c;
            registry.add<NameComponent>(entity, c);
        }
        if (!registry.has<Transform2DComponent>(entity) && ImGui::Selectable("Transform 2D")) {
            Transform2DComponent c;
            registry.add<Transform2DComponent>(entity, c);
        }
        if (!registry.has<Transform3DComponent>(entity) && ImGui::Selectable("Transform 3D")) {
            Transform3DComponent c;
            registry.add<Transform3DComponent>(entity, c);
        }
        if (!registry.has<ModelComponent>(entity) && ImGui::Selectable("Model")) {
            ModelComponent c;
            registry.add<ModelComponent>(entity, c);
        }
        if (!registry.has<RigidBody3DComponent>(entity) && ImGui::Selectable("Rigid Body 3D")) {
            RigidBody3DComponent c;
            registry.add<RigidBody3DComponent>(entity, c);
        }
        if (!registry.has<BoxCollider3DComponent>(entity) && ImGui::Selectable("Box Collider 3D")) {
            BoxCollider3DComponent c;
            registry.add<BoxCollider3DComponent>(entity, c);
        }
        if (!registry.has<SphereCollider3DComponent>(entity) && ImGui::Selectable("Sphere Collider 3D")) {
            SphereCollider3DComponent c;
            registry.add<SphereCollider3DComponent>(entity, c);
        }
        if (!registry.has<CapsuleCollider3DComponent>(entity) && ImGui::Selectable("Capsule Collider 3D")) {
            CapsuleCollider3DComponent c;
            registry.add<CapsuleCollider3DComponent>(entity, c);
        }
        if (!registry.has<ScriptComponent>(entity) && ImGui::Selectable("Script")) {
            ScriptComponent c;
            registry.add<ScriptComponent>(entity, c);
        }
        if (!registry.has<Camera3DComponent>(entity) && ImGui::Selectable("Camera 3D")) {
            Camera3DComponent c;
            registry.add<Camera3DComponent>(entity, c);
        }
        if (!registry.has<PathComponent>(entity) && ImGui::Selectable("Path")) {
            PathComponent c;
            registry.add<PathComponent>(entity, c);
        }
        if (!registry.has<LightComponent>(entity) && ImGui::Selectable("Light")) {
            LightComponent c;
            registry.add<LightComponent>(entity, c);
        }
        if (!registry.has<SunComponent>(entity) && ImGui::Selectable("Sun")) {
            SunComponent c;
            registry.add<SunComponent>(entity, c);
        }
        if (!registry.has<AmbienceComponent>(entity) && ImGui::Selectable("Ambience")) {
            AmbienceComponent c;
            registry.add<AmbienceComponent>(entity, c);
        }
        if (!registry.has<ParticleComponent>(entity) && ImGui::Selectable("Particle")) {
            ParticleComponent c;
            registry.add<ParticleComponent>(entity, c);
        }
        if (!registry.has<MeshAnimationComponent>(entity) && ImGui::Selectable("Mesh Animation")) {
            MeshAnimationComponent c;
            registry.add<MeshAnimationComponent>(entity, c);
        }
        if (!registry.has<CollisionEvent>(entity) && ImGui::Selectable("Collision Event")) {
            CollisionEvent c;
            registry.add<CollisionEvent>(entity, c);
        }
        if (!registry.has<PhysicsSingleton>(entity) && ImGui::Selectable("Physics Singleton")) {
            PhysicsSingleton c;
            registry.add<PhysicsSingleton>(entity, c);
        }
        if (!registry.has<InputSingleton>(entity) && ImGui::Selectable("Input Singleton")) {
            InputSingleton c;
            registry.add<InputSingleton>(entity, c);
        }
        if (!registry.has<GameGridSingleton>(entity) && ImGui::Selectable("Game Grid Singleton")) {
            GameGridSingleton c;
            registry.add<GameGridSingleton>(entity, c);
        }
        if (!registry.has<TileMapSingleton>(entity) && ImGui::Selectable("Tile Map Singleton")) {
            TileMapSingleton c;
            registry.add<TileMapSingleton>(entity, c);
        }
        if (!registry.has<CameraSingleton>(entity) && ImGui::Selectable("Camera Singleton")) {
            CameraSingleton c;
            registry.add<CameraSingleton>(entity, c);
        }
        if (!registry.has<ParticleSingleton>(entity) && ImGui::Selectable("Particle Singleton")) {
            ParticleSingleton c;
            registry.add<ParticleSingleton>(entity, c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        apx::entity copy = spkt::copy_entity(&editor.active_scene()->Entities(), entity);
        editor.set_selected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        registry.destroy(entity);
        editor.clear_selected();
    }
}

}
