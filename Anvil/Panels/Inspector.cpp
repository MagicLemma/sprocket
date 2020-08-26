#include "Inspector.h"
#include "EditorLayer.h"
#include "ImGuiHelpers.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>

namespace Sprocket {

void ShowInspector(EditorLayer& editor)
{
    Entity entity = editor.Selected();

    if (editor.Selected().Null()) {
        if (ImGui::Button("New Entity")) {
            auto e = editor.GetScene()->NewEntity();
            editor.SetSelected(e);
        }
        return;
    }

    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;
    static DevUI::GizmoMode mode = DevUI::GizmoMode::ROTATION;

    if (entity.Has<NameComponent>()) {
        if (ImGui::CollapsingHeader("Name")) {
            ImGui::PushID(0);
            auto& c = entity.Get<NameComponent>();
            ImGuiXtra::TextModifiable(c.name);
            if (ImGui::Button("Delete")) { entity.Remove<NameComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<TransformComponent>()) {
        auto& c = entity.Get<TransformComponent>();
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::PushID(1);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGuiXtra::GuizmoSettings(mode, coords);
            if (ImGui::Button("Delete")) { entity.Remove<TransformComponent>(); }
            ImGui::PopID();
        }

        if (!editor.IsGameRunning()) {
            auto& camera = editor.GetEditorCamera();
            auto tr = Maths::Transform(c.position, c.orientation);
            ImGuizmo::Manipulate(
                Maths::Cast(camera.View()),
                Maths::Cast(camera.Proj()),
                GetMode(mode),
                GetCoords(coords),
                Maths::Cast(tr)
            );
            c.position = Maths::GetTranslation(tr);
            c.orientation = Maths::ToQuat(Maths::mat3(tr));
            Maths::Normalise(c.orientation);
        }
    }
    if (entity.Has<ModelComponent>()) {
        if (ImGui::CollapsingHeader("Model")) {
            ImGui::PushID(2);
            auto& c = entity.Get<ModelComponent>();
            ImGuiXtra::File("Model", editor.GetWindow(), &c.model, "*.obj");
            ImGui::DragFloat("Scale", &c.scale, 0.1f);
            ImGuiXtra::File("Texture", editor.GetWindow(), &c.texture, "*.png");
            ImGui::DragFloat("Shine Damper", &c.shineDamper, 0.1f);
            ImGui::DragFloat("Reflectivity", &c.reflectivity, 0.1f);
            if (ImGui::Button("Delete")) { entity.Remove<ModelComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<RigidBody3DComponent>()) {
        if (ImGui::CollapsingHeader("RigidBody3D")) {
            ImGui::PushID(3);
            auto& c = entity.Get<RigidBody3DComponent>();
            ImGui::DragFloat3("Velocity", &c.velocity.x);
            ImGui::Checkbox("Gravity", &c.gravity);
            ImGui::Checkbox("Frozen", &c.frozen);
            ImGui::SliderFloat("Bounciness", &c.bounciness, 0.0, 1.0);
            ImGui::SliderFloat("Friction Coefficient", &c.frictionCoefficient, 0.0, 1.0);
            ImGui::SliderFloat("Rolling Resistance", &c.rollingResistance, 0.0, 1.0);
            if (ImGui::Button("Delete")) { entity.Remove<RigidBody3DComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<BoxCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("BoxCollider3D")) {
            ImGui::PushID(4);
            auto& c = entity.Get<BoxCollider3DComponent>();
            ImGui::DragFloat3("Position", &c.position.x, 0.01f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.01f);
            if (ImGui::Button("Delete")) { entity.Remove<BoxCollider3DComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<SphereCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("SphereCollider3D")) {
            ImGui::PushID(5);
            auto& c = entity.Get<SphereCollider3DComponent>();
            ImGui::DragFloat3("Position", &c.position.x, 0.01f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            if (ImGui::Button("Delete")) { entity.Remove<SphereCollider3DComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<CapsuleCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("CapsuleCollider3D")) {
            ImGui::PushID(6);
            auto& c = entity.Get<CapsuleCollider3DComponent>();
            ImGui::DragFloat3("Position", &c.position.x, 0.01f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
            if (ImGui::Button("Delete")) { entity.Remove<CapsuleCollider3DComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<ScriptComponent>()) {
        if (ImGui::CollapsingHeader("Script")) {
            ImGui::PushID(7);
            auto& c = entity.Get<ScriptComponent>();
            ImGuiXtra::File("Script", editor.GetWindow(), &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            if (ImGui::Button("Delete")) { entity.Remove<ScriptComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<CameraComponent>()) {
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::PushID(8);
            auto& c = entity.Get<CameraComponent>();
            if (ImGui::Button("Delete")) { entity.Remove<CameraComponent>(); }
            ImGui::PopID();
        }
    }
    if (entity.Has<LightComponent>()) {
        if (ImGui::CollapsingHeader("Light")) {
            ImGui::PushID(9);
            auto& c = entity.Get<LightComponent>();
            ImGui::ColorPicker3("Colour", &c.colour.r);
            ImGui::DragFloat3("Attenuation", &c.attenuation.x);
            ImGui::DragFloat("Brightness", &c.brightness);
            if (ImGui::Button("Delete")) { entity.Remove<LightComponent>(); }
            ImGui::PopID();
        }
    }
    
    ImGui::Separator();

    if (ImGui::BeginMenu("Add Component")) {
        if (!entity.Has<NameComponent>() && ImGui::MenuItem("Name")) {
            NameComponent c;
            entity.Add(c);
        }
        if (!entity.Has<TransformComponent>() && ImGui::MenuItem("Transform")) {
            TransformComponent c;
            entity.Add(c);
        }
        if (!entity.Has<RigidBody3DComponent>() && ImGui::MenuItem("Rigid Body 3D")) {
            RigidBody3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<BoxCollider3DComponent>() && ImGui::MenuItem("Box Collider 3D")) {
            BoxCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<SphereCollider3DComponent>() && ImGui::MenuItem("Sphere Collider 3D")) {
            SphereCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<CapsuleCollider3DComponent>() && ImGui::MenuItem("Capsule Collider 3D")) {
            CapsuleCollider3DComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ModelComponent>() && ImGui::MenuItem("Model")) {
            ModelComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ScriptComponent>() && ImGui::MenuItem("Script")) {
            ScriptComponent c;
            entity.Add(c);
        }
        if (!entity.Has<LightComponent>() && ImGui::MenuItem("Light")) {
            LightComponent c;
            entity.Add(c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
        editor.ClearSelected();
    }
}
    
}