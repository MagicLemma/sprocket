// GENERATED FILE
#include "Inspector.h"
#include "EditorLayer.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>

namespace Sprocket {
namespace {

void ShowGuizmo(
    EditorLayer& editor,
    TransformComponent& c,
    DevUI::GizmoMode mode,
    DevUI::GizmoCoords coords,
    Maths::vec3* snap = nullptr)
{
    if (!editor.IsGameRunning()) {
        auto& camera = editor.GetEditorCamera();
        auto tr = Maths::Transform(c.position, c.orientation, c.scale);
        ImGuizmo::Manipulate(
            Maths::Cast(camera.View()),
            Maths::Cast(camera.Proj()),
            GetMode(mode),
            GetCoords(coords),
            Maths::Cast(tr),
            nullptr,
            &snap->x
        );
        Maths::Decompose(tr, &c.position, &c.orientation, &c.scale);
    }
}

}

void Inspector::Show(EditorLayer& editor)
{
    Entity entity = editor.Selected();

    if (editor.Selected().Null()) {
        if (ImGui::Button("New Entity")) {
            auto e = editor.GetScene()->NewEntity();
            editor.SetSelected(e);
        }
        return;
    }
    int count = 0;

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

    if (entity.Has<TransformComponent>()) {
        auto& c = entity.Get<TransformComponent>();
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::PushID(count++);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGuiXtra::GuizmoSettings(d_mode, d_coords, d_useSnap, d_snap);
            if (ImGui::Button("Delete")) { entity.Remove<TransformComponent>(); }
            ImGui::PopID();
        }
        ShowGuizmo(editor, c, d_mode, d_coords, d_useSnap ? &d_snap : nullptr);
    }

    if (entity.Has<ModelComponent>()) {
        auto& c = entity.Get<ModelComponent>();
        if (ImGui::CollapsingHeader("Model")) {
            ImGui::PushID(count++);
            ImGuiXtra::File("Model", editor.GetWindow(), &c.model, "*.obj");
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
            ImGui::DragFloat("Mass", &c.mass, 0.1f);
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
            ImGui::DragFloat("Mass", &c.mass, 0.1f);
            ImGui::DragFloat("Radius", &c.radius, 0.1f);
            
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
            ImGui::DragFloat("Mass", &c.mass, 0.1f);
            ImGui::DragFloat("Radius", &c.radius, 0.1f);
            ImGui::DragFloat("Height", &c.height, 0.1f);
            
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

    if (entity.Has<CameraComponent>()) {
        auto& c = entity.Get<CameraComponent>();
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::PushID(count++);
            ;
            ImGui::DragFloat("FOV", &c.fov, 0.1f);
            ImGui::DragFloat("Pitch", &c.pitch, 0.1f);
            
            if (ImGui::Button("Delete")) { entity.Remove<CameraComponent>(); }
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
            ImGui::DragFloat("Speed", &c.speed, 0.1f);
            
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
            ImGui::ColorPicker3("Colour", &c.colour.r);
            ImGui::DragFloat("Brightness", &c.brightness, 0.1f);
            
            if (ImGui::Button("Delete")) { entity.Remove<LightComponent>(); }
            ImGui::PopID();
        }
        
    }

    if (entity.Has<ParticleComponent>()) {
        auto& c = entity.Get<ParticleComponent>();
        if (ImGui::CollapsingHeader("Particle")) {
            ImGui::PushID(count++);
            ImGui::DragFloat("Interval", &c.interval, 0.1f);
            ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
            ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.1f);
            ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGui::DragFloat("Life", &c.life, 0.1f);
            ImGui::DragFloat("Accumulator", &c.accumulator, 0.1f);
            
            if (ImGui::Button("Delete")) { entity.Remove<ParticleComponent>(); }
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
            entity.Add(c);
        }
        if (!entity.Has<NameComponent>() && ImGui::Selectable("Name")) {
            NameComponent c;
            entity.Add(c);
        }
        if (!entity.Has<TransformComponent>() && ImGui::Selectable("Transform")) {
            TransformComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ModelComponent>() && ImGui::Selectable("Model")) {
            ModelComponent c;
            entity.Add(c);
        }
        if (!entity.Has<RigidBody3DComponent>() && ImGui::Selectable("Rigid Body 3D")) {
            RigidBody3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<BoxCollider3DComponent>() && ImGui::Selectable("Box Collider 3D")) {
            BoxCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<SphereCollider3DComponent>() && ImGui::Selectable("Sphere Collider 3D")) {
            SphereCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<CapsuleCollider3DComponent>() && ImGui::Selectable("Capsule Collider 3D")) {
            CapsuleCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ScriptComponent>() && ImGui::Selectable("Script")) {
            ScriptComponent c;
            entity.Add(c);
        }
        if (!entity.Has<CameraComponent>() && ImGui::Selectable("Camera")) {
            CameraComponent c;
            entity.Add(c);
        }
        if (!entity.Has<SelectComponent>() && ImGui::Selectable("Select")) {
            SelectComponent c;
            entity.Add(c);
        }
        if (!entity.Has<PathComponent>() && ImGui::Selectable("Path")) {
            PathComponent c;
            entity.Add(c);
        }
        if (!entity.Has<GridComponent>() && ImGui::Selectable("Grid")) {
            GridComponent c;
            entity.Add(c);
        }
        if (!entity.Has<LightComponent>() && ImGui::Selectable("Light")) {
            LightComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ParticleComponent>() && ImGui::Selectable("Particle")) {
            ParticleComponent c;
            entity.Add(c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        Entity copy = Loader::Copy(editor.GetScene(), entity);
        editor.SetSelected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
        editor.ClearSelected();
    }
}

}
