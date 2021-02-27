// GENERATED FILE
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
    ecs::Entity entity = editor.Selected();

    if (!editor.Selected().Valid()) {
        if (ImGui::Button("New Entity")) {
            auto e = editor.GetScene()->Entities().New();
            editor.SetSelected(e);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), guid::Stringify(entity.Id()).c_str());

    if (entity.Has<TemporaryComponent>()) {
        auto& c = entity.Get<TemporaryComponent>();
        if (ImGui::CollapsingHeader("Temporary")) {
            ImGui::PushID(count++);
            
            if (ImGui::Button("Delete")) { entity.Remove<TemporaryComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<NameComponent>()) {
        auto& c = entity.Get<NameComponent>();
        if (ImGui::CollapsingHeader("Name")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            
            if (ImGui::Button("Delete")) { entity.Remove<NameComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<Transform2DComponent>()) {
        auto& c = entity.Get<Transform2DComponent>();
        if (ImGui::CollapsingHeader("Transform 2D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat2("Position", &c.position.x, 0.1f);
            ImGui::DragFloat("Rotation", &c.rotation, 0.01f);
            ImGui::DragFloat2("Scale", &c.scale.x, 0.1f);
            
            if (ImGui::Button("Delete")) { entity.Remove<Transform2DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<Transform3DComponent>()) {
        auto& c = entity.Get<Transform3DComponent>();
        if (ImGui::CollapsingHeader("Transform 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
            if (ImGui::Button("Delete")) { entity.Remove<Transform3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<ModelComponent>()) {
        auto& c = entity.Get<ModelComponent>();
        if (ImGui::CollapsingHeader("Model")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Mesh", editor.GetWindow(), &c.mesh, "*.obj");
            ImGuiXtra::File("Material", editor.GetWindow(), &c.material, "*.yaml");
            
            if (ImGui::Button("Delete")) { entity.Remove<ModelComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<RigidBody3DComponent>()) {
        auto& c = entity.Get<RigidBody3DComponent>();
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
            
            if (ImGui::Button("Delete")) { entity.Remove<RigidBody3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<BoxCollider3DComponent>()) {
        auto& c = entity.Get<BoxCollider3DComponent>();
        if (ImGui::CollapsingHeader("Box Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
            ImGui::Checkbox("Apply Scale", &c.applyScale);
            
            if (ImGui::Button("Delete")) { entity.Remove<BoxCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<SphereCollider3DComponent>()) {
        auto& c = entity.Get<SphereCollider3DComponent>();
        if (ImGui::CollapsingHeader("Sphere Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<SphereCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<CapsuleCollider3DComponent>()) {
        auto& c = entity.Get<CapsuleCollider3DComponent>();
        if (ImGui::CollapsingHeader("Capsule Collider 3D")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Mass", &c.mass, 0.01f);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<CapsuleCollider3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<ScriptComponent>()) {
        auto& c = entity.Get<ScriptComponent>();
        if (ImGui::CollapsingHeader("Script")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Script", editor.GetWindow(), &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            
            if (ImGui::Button("Delete")) { entity.Remove<ScriptComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<Camera3DComponent>()) {
        auto& c = entity.Get<Camera3DComponent>();
        if (ImGui::CollapsingHeader("Camera 3D")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("FOV", &c.fov, 0.01f);
            ImGui::DragFloat("Pitch", &c.pitch, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<Camera3DComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<SelectComponent>()) {
        auto& c = entity.Get<SelectComponent>();
        if (ImGui::CollapsingHeader("Select")) {
            ImGui::PushID(count++);
            ImGui::Checkbox("Selected", &c.selected);
            ImGui::Checkbox("Hovered", &c.hovered);
            
            if (ImGui::Button("Delete")) { entity.Remove<SelectComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<PathComponent>()) {
        auto& c = entity.Get<PathComponent>();
        if (ImGui::CollapsingHeader("Path")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<PathComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<GridComponent>()) {
        auto& c = entity.Get<GridComponent>();
        if (ImGui::CollapsingHeader("Grid")) {
            ImGui::PushID(count++);
            ;
            ;
            
            if (ImGui::Button("Delete")) { entity.Remove<GridComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<LightComponent>()) {
        auto& c = entity.Get<LightComponent>();
        if (ImGui::CollapsingHeader("Light")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<LightComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<SunComponent>()) {
        auto& c = entity.Get<SunComponent>();
        if (ImGui::CollapsingHeader("Sun")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            ImGui::DragFloat3("Direction", &c.direction.x, 0.1f);
            ImGui::Checkbox("Shadows", &c.shadows);
            
            if (ImGui::Button("Delete")) { entity.Remove<SunComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<AmbienceComponent>()) {
        auto& c = entity.Get<AmbienceComponent>();
        if (ImGui::CollapsingHeader("Ambience")) {
            ImGui::PushID(count++);
            ImGui::ColorEdit3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<AmbienceComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<ParticleComponent>()) {
        auto& c = entity.Get<ParticleComponent>();
        if (ImGui::CollapsingHeader("Particle")) {
            ImGui::PushID(count++);
            ImGui::DragFloat("Interval", &c.interval, 0.01f);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.01f);
            ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGui::DragFloat("Life", &c.life, 0.01f);
            ImGui::DragFloat("Accumulator", &c.accumulator, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<ParticleComponent>(); }
            ImGui::PopID();
        }
    }

    if (entity.Has<MeshAnimationComponent>()) {
        auto& c = entity.Get<MeshAnimationComponent>();
        if (ImGui::CollapsingHeader("Mesh Animation")) {
            ImGui::PushID(count++);
            ImGuiXtra::TextModifiable(c.name);
            ImGui::DragFloat("Time", &c.time, 0.01f);
            ImGui::DragFloat("Speed", &c.speed, 0.01f);
            
            if (ImGui::Button("Delete")) { entity.Remove<MeshAnimationComponent>(); }
            ImGui::PopID();
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        if (!entity.Has<TemporaryComponent>() && ImGui::Selectable("Temporary")) {
            TemporaryComponent c;
            entity.Add<TemporaryComponent>(c);
        }
        if (!entity.Has<NameComponent>() && ImGui::Selectable("Name")) {
            NameComponent c;
            entity.Add<NameComponent>(c);
        }
        if (!entity.Has<Transform2DComponent>() && ImGui::Selectable("Transform 2D")) {
            Transform2DComponent c;
            entity.Add<Transform2DComponent>(c);
        }
        if (!entity.Has<Transform3DComponent>() && ImGui::Selectable("Transform 3D")) {
            Transform3DComponent c;
            entity.Add<Transform3DComponent>(c);
        }
        if (!entity.Has<ModelComponent>() && ImGui::Selectable("Model")) {
            ModelComponent c;
            entity.Add<ModelComponent>(c);
        }
        if (!entity.Has<RigidBody3DComponent>() && ImGui::Selectable("Rigid Body 3D")) {
            RigidBody3DComponent c;
            entity.Add<RigidBody3DComponent>(c);
        }
        if (!entity.Has<BoxCollider3DComponent>() && ImGui::Selectable("Box Collider 3D")) {
            BoxCollider3DComponent c;
            entity.Add<BoxCollider3DComponent>(c);
        }
        if (!entity.Has<SphereCollider3DComponent>() && ImGui::Selectable("Sphere Collider 3D")) {
            SphereCollider3DComponent c;
            entity.Add<SphereCollider3DComponent>(c);
        }
        if (!entity.Has<CapsuleCollider3DComponent>() && ImGui::Selectable("Capsule Collider 3D")) {
            CapsuleCollider3DComponent c;
            entity.Add<CapsuleCollider3DComponent>(c);
        }
        if (!entity.Has<ScriptComponent>() && ImGui::Selectable("Script")) {
            ScriptComponent c;
            entity.Add<ScriptComponent>(c);
        }
        if (!entity.Has<Camera3DComponent>() && ImGui::Selectable("Camera 3D")) {
            Camera3DComponent c;
            entity.Add<Camera3DComponent>(c);
        }
        if (!entity.Has<SelectComponent>() && ImGui::Selectable("Select")) {
            SelectComponent c;
            entity.Add<SelectComponent>(c);
        }
        if (!entity.Has<PathComponent>() && ImGui::Selectable("Path")) {
            PathComponent c;
            entity.Add<PathComponent>(c);
        }
        if (!entity.Has<GridComponent>() && ImGui::Selectable("Grid")) {
            GridComponent c;
            entity.Add<GridComponent>(c);
        }
        if (!entity.Has<LightComponent>() && ImGui::Selectable("Light")) {
            LightComponent c;
            entity.Add<LightComponent>(c);
        }
        if (!entity.Has<SunComponent>() && ImGui::Selectable("Sun")) {
            SunComponent c;
            entity.Add<SunComponent>(c);
        }
        if (!entity.Has<AmbienceComponent>() && ImGui::Selectable("Ambience")) {
            AmbienceComponent c;
            entity.Add<AmbienceComponent>(c);
        }
        if (!entity.Has<ParticleComponent>() && ImGui::Selectable("Particle")) {
            ParticleComponent c;
            entity.Add<ParticleComponent>(c);
        }
        if (!entity.Has<MeshAnimationComponent>() && ImGui::Selectable("Mesh Animation")) {
            MeshAnimationComponent c;
            entity.Add<MeshAnimationComponent>(c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        ecs::Entity copy = Loader::Copy(&editor.GetScene()->Entities(), entity);
        editor.SetSelected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        entity.Delete();
        editor.ClearSelected();
    }
}

}
