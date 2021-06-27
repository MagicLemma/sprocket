#include "Inspector.h"
#include "Anvil.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Sprocket {

void Inspector::Show(Anvil& editor)
{
    spkt::entity entity = editor.selected();

    if (!entity.valid()) {
        if (ImGui::Button("New Entity")) {
            auto e = apx::create_from(editor.active_scene()->Entities());
            editor.set_selected(e);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity.entity());

    if (entity.has<TemporaryComponent>()) {
        auto& c = entity.get<TemporaryComponent>();
        if (ImGui::CollapsingHeader("Temporary")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { entity.remove<TemporaryComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<NameComponent>()) {
        auto& c = entity.get<NameComponent>();
        if (ImGui::CollapsingHeader("Name")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            
            if (ImGui::Button("Delete")) { entity.remove<NameComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<Transform2DComponent>()) {
        auto& c = entity.get<Transform2DComponent>();
        if (ImGui::CollapsingHeader("Transform 2D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat2("Position", &c.position.x, 0.1f);
            ImGui::DragFloat("Rotation", &c.rotation, 0.01f);
            ImGui::DragFloat2("Scale", &c.scale.x, 0.1f);
            
            if (ImGui::Button("Delete")) { entity.remove<Transform2DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<Transform3DComponent>()) {
        auto& c = entity.get<Transform3DComponent>();
        if (ImGui::CollapsingHeader("Transform 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
            if (ImGui::Button("Delete")) { entity.remove<Transform3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<ModelComponent>()) {
        auto& c = entity.get<ModelComponent>();
        if (ImGui::CollapsingHeader("Model")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
            ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
            
            if (ImGui::Button("Delete")) { entity.remove<ModelComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<RigidBody3DComponent>()) {
        auto& c = entity.get<RigidBody3DComponent>();
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
            
            if (ImGui::Button("Delete")) { entity.remove<RigidBody3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<BoxCollider3DComponent>()) {
        auto& c = entity.get<BoxCollider3DComponent>();
        if (ImGui::CollapsingHeader("Box Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
            ImGui::Checkbox("Apply Scale", &c.applyScale);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<BoxCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<SphereCollider3DComponent>()) {
        auto& c = entity.get<SphereCollider3DComponent>();
        if (ImGui::CollapsingHeader("Sphere Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<SphereCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<CapsuleCollider3DComponent>()) {
        auto& c = entity.get<CapsuleCollider3DComponent>();
        if (ImGui::CollapsingHeader("Capsule Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<CapsuleCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<ScriptComponent>()) {
        auto& c = entity.get<ScriptComponent>();
        if (ImGui::CollapsingHeader("Script")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Script", editor.window(), &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<ScriptComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<Camera3DComponent>()) {
        auto& c = entity.get<Camera3DComponent>();
        if (ImGui::CollapsingHeader("Camera 3D")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("FOV", &c.fov, 0.01f);
            ImGui::DragFloat("Pitch", &c.pitch, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<Camera3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<SelectComponent>()) {
        auto& c = entity.get<SelectComponent>();
        if (ImGui::CollapsingHeader("Select")) {
            ImGui::PushID(count++);
            ImGui::Checkbox("Selected", &c.selected);
            ImGui::Checkbox("Hovered", &c.hovered);
            
            if (ImGui::Button("Delete")) { entity.remove<SelectComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<PathComponent>()) {
        auto& c = entity.get<PathComponent>();
        if (ImGui::CollapsingHeader("Path")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<PathComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<GridComponent>()) {
        auto& c = entity.get<GridComponent>();
        if (ImGui::CollapsingHeader("Grid")) {
            ImGui::PushID(count++);
            ;
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<GridComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<LightComponent>()) {
        auto& c = entity.get<LightComponent>();
        if (ImGui::CollapsingHeader("Light")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<LightComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<SunComponent>()) {
        auto& c = entity.get<SunComponent>();
        if (ImGui::CollapsingHeader("Sun")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            ImGui::DragFloat3("Direction", &c.direction.x, 0.1f);
            ImGui::Checkbox("Shadows", &c.shadows);
            
            if (ImGui::Button("Delete")) { entity.remove<SunComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<AmbienceComponent>()) {
        auto& c = entity.get<AmbienceComponent>();
        if (ImGui::CollapsingHeader("Ambience")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<AmbienceComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<ParticleComponent>()) {
        auto& c = entity.get<ParticleComponent>();
        if (ImGui::CollapsingHeader("Particle")) {
            ImGui::PushID(count++);
            ImGui::DragFloat("Interval", &c.interval, 0.01f);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.01f);
            ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGui::DragFloat("Life", &c.life, 0.01f);
            ImGui::DragFloat("Accumulator", &c.accumulator, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<ParticleComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<MeshAnimationComponent>()) {
        auto& c = entity.get<MeshAnimationComponent>();
        if (ImGui::CollapsingHeader("Mesh Animation")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            ImGui::DragFloat("Time", &c.time, 0.01f);
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.remove<MeshAnimationComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<Singleton>()) {
        auto& c = entity.get<Singleton>();
        if (ImGui::CollapsingHeader("Singleton")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { entity.remove<Singleton>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<CollisionSingleton>()) {
        auto& c = entity.get<CollisionSingleton>();
        if (ImGui::CollapsingHeader("Collision Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<CollisionSingleton>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<InputSingleton>()) {
        auto& c = entity.get<InputSingleton>();
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
            
            if (ImGui::Button("Delete")) { entity.remove<InputSingleton>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<GameGridSingleton>()) {
        auto& c = entity.get<GameGridSingleton>();
        if (ImGui::CollapsingHeader("Game Grid Singleton")) {
            ImGui::PushID(count++);
            ;
            ;
            ;
            ;
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<GameGridSingleton>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<TileMapSingleton>()) {
        auto& c = entity.get<TileMapSingleton>();
        if (ImGui::CollapsingHeader("Tile Map Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<TileMapSingleton>(); }
            ImGui::PopID();
        }
    }

    if (entity.has<CameraSingleton>()) {
        auto& c = entity.get<CameraSingleton>();
        if (ImGui::CollapsingHeader("Camera Singleton")) {
            ImGui::PushID(count++);
            ;
            
            if (ImGui::Button("Delete")) { entity.remove<CameraSingleton>(); }
            ImGui::PopID();
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        if (!entity.has<TemporaryComponent>() && ImGui::Selectable("Temporary")) {
            TemporaryComponent c;
            entity.add<TemporaryComponent>(c);
        }
        if (!entity.has<NameComponent>() && ImGui::Selectable("Name")) {
            NameComponent c;
            entity.add<NameComponent>(c);
        }
        if (!entity.has<Transform2DComponent>() && ImGui::Selectable("Transform 2D")) {
            Transform2DComponent c;
            entity.add<Transform2DComponent>(c);
        }
        if (!entity.has<Transform3DComponent>() && ImGui::Selectable("Transform 3D")) {
            Transform3DComponent c;
            entity.add<Transform3DComponent>(c);
        }
        if (!entity.has<ModelComponent>() && ImGui::Selectable("Model")) {
            ModelComponent c;
            entity.add<ModelComponent>(c);
        }
        if (!entity.has<RigidBody3DComponent>() && ImGui::Selectable("Rigid Body 3D")) {
            RigidBody3DComponent c;
            entity.add<RigidBody3DComponent>(c);
        }
        if (!entity.has<BoxCollider3DComponent>() && ImGui::Selectable("Box Collider 3D")) {
            BoxCollider3DComponent c;
            entity.add<BoxCollider3DComponent>(c);
        }
        if (!entity.has<SphereCollider3DComponent>() && ImGui::Selectable("Sphere Collider 3D")) {
            SphereCollider3DComponent c;
            entity.add<SphereCollider3DComponent>(c);
        }
        if (!entity.has<CapsuleCollider3DComponent>() && ImGui::Selectable("Capsule Collider 3D")) {
            CapsuleCollider3DComponent c;
            entity.add<CapsuleCollider3DComponent>(c);
        }
        if (!entity.has<ScriptComponent>() && ImGui::Selectable("Script")) {
            ScriptComponent c;
            entity.add<ScriptComponent>(c);
        }
        if (!entity.has<Camera3DComponent>() && ImGui::Selectable("Camera 3D")) {
            Camera3DComponent c;
            entity.add<Camera3DComponent>(c);
        }
        if (!entity.has<SelectComponent>() && ImGui::Selectable("Select")) {
            SelectComponent c;
            entity.add<SelectComponent>(c);
        }
        if (!entity.has<PathComponent>() && ImGui::Selectable("Path")) {
            PathComponent c;
            entity.add<PathComponent>(c);
        }
        if (!entity.has<GridComponent>() && ImGui::Selectable("Grid")) {
            GridComponent c;
            entity.add<GridComponent>(c);
        }
        if (!entity.has<LightComponent>() && ImGui::Selectable("Light")) {
            LightComponent c;
            entity.add<LightComponent>(c);
        }
        if (!entity.has<SunComponent>() && ImGui::Selectable("Sun")) {
            SunComponent c;
            entity.add<SunComponent>(c);
        }
        if (!entity.has<AmbienceComponent>() && ImGui::Selectable("Ambience")) {
            AmbienceComponent c;
            entity.add<AmbienceComponent>(c);
        }
        if (!entity.has<ParticleComponent>() && ImGui::Selectable("Particle")) {
            ParticleComponent c;
            entity.add<ParticleComponent>(c);
        }
        if (!entity.has<MeshAnimationComponent>() && ImGui::Selectable("Mesh Animation")) {
            MeshAnimationComponent c;
            entity.add<MeshAnimationComponent>(c);
        }
        if (!entity.has<Singleton>() && ImGui::Selectable("Singleton")) {
            Singleton c;
            entity.add<Singleton>(c);
        }
        if (!entity.has<CollisionSingleton>() && ImGui::Selectable("Collision Singleton")) {
            CollisionSingleton c;
            entity.add<CollisionSingleton>(c);
        }
        if (!entity.has<InputSingleton>() && ImGui::Selectable("Input Singleton")) {
            InputSingleton c;
            entity.add<InputSingleton>(c);
        }
        if (!entity.has<GameGridSingleton>() && ImGui::Selectable("Game Grid Singleton")) {
            GameGridSingleton c;
            entity.add<GameGridSingleton>(c);
        }
        if (!entity.has<TileMapSingleton>() && ImGui::Selectable("Tile Map Singleton")) {
            TileMapSingleton c;
            entity.add<TileMapSingleton>(c);
        }
        if (!entity.has<CameraSingleton>() && ImGui::Selectable("Camera Singleton")) {
            CameraSingleton c;
            entity.add<CameraSingleton>(c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = Loader::Copy(&editor.active_scene()->Entities(), entity);
        editor.set_selected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        entity.destroy();
        editor.clear_selected();
    }
}

}
