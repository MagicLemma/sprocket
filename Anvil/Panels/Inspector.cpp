// GENERATED FILE @ 2020-08-30 16:16:44.739564

#include "Inspector.h"
#include "EditorLayer.h"
#include "ImGuiXtra.h"
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
    if (entity.Has<TemporaryComponent>() && ImGui::CollapsingHeader("Temporary")) {
        ImGui::PushID(0);
        auto& c = entity.Get<TemporaryComponent>();
        if (ImGui::Button("Delete")) { entity.Remove<TemporaryComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<NameComponent>() && ImGui::CollapsingHeader("Name")) {
        ImGui::PushID(1);
        auto& c = entity.Get<NameComponent>();
        ImGuiXtra::TextModifiable(c.name);
        if (ImGui::Button("Delete")) { entity.Remove<NameComponent>(); }
        ImGui::PopID();
    }

    if (entity.Has<TransformComponent>()) {
        auto& c = entity.Get<TransformComponent>();
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::PushID(1000000);
            ImGui::DragFloat3("Position", &c.position.x, 0.1f);
            ImGuiXtra::Euler("Orientation", &c.orientation);
            ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
            ImGuiXtra::GuizmoSettings(mode, coords);
            if (ImGui::Button("Delete")) { entity.Remove<TransformComponent>(); }
            ImGui::PopID();
        }

        if (!editor.IsGameRunning()) {
            auto& camera = editor.GetEditorCamera();
            auto tr = Maths::Transform(c.position, c.orientation, c.scale);
            ImGuizmo::Manipulate(
                Maths::Cast(camera.View()),
                Maths::Cast(camera.Proj()),
                GetMode(mode),
                GetCoords(coords),
                Maths::Cast(tr)
            );
            Maths::Decompose(tr, &c.position, &c.orientation, &c.scale);
        }
    }
    if (entity.Has<ModelComponent>() && ImGui::CollapsingHeader("Model")) {
        ImGui::PushID(3);
        auto& c = entity.Get<ModelComponent>();
        ImGuiXtra::File("Model", editor.GetWindow(), &c.model, "*.obj");
        ImGuiXtra::File("Texture", editor.GetWindow(), &c.texture, "*.png");
        ImGui::DragFloat("Shine Damper", &c.shineDamper, 0.1f);
        ImGui::DragFloat("Reflectivity", &c.reflectivity, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<ModelComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<RigidBody3DComponent>() && ImGui::CollapsingHeader("Rigid Body 3D")) {
        ImGui::PushID(4);
        auto& c = entity.Get<RigidBody3DComponent>();
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
    if (entity.Has<BoxCollider3DComponent>() && ImGui::CollapsingHeader("Box Collider 3D")) {
        ImGui::PushID(5);
        auto& c = entity.Get<BoxCollider3DComponent>();
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.1f);
        ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
        ImGui::Checkbox("Apply Scale", &c.applyScale);
        if (ImGui::Button("Delete")) { entity.Remove<BoxCollider3DComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<SphereCollider3DComponent>() && ImGui::CollapsingHeader("Sphere Collider 3D")) {
        ImGui::PushID(6);
        auto& c = entity.Get<SphereCollider3DComponent>();
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.1f);
        ImGui::DragFloat("Radius", &c.radius, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<SphereCollider3DComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<CapsuleCollider3DComponent>() && ImGui::CollapsingHeader("Capsule Collider 3D")) {
        ImGui::PushID(7);
        auto& c = entity.Get<CapsuleCollider3DComponent>();
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.1f);
        ImGui::DragFloat("Radius", &c.radius, 0.1f);
        ImGui::DragFloat("Height", &c.height, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<CapsuleCollider3DComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<ScriptComponent>() && ImGui::CollapsingHeader("Script")) {
        ImGui::PushID(8);
        auto& c = entity.Get<ScriptComponent>();
        ImGuiXtra::File("Script", editor.GetWindow(), &c.script, "*.lua");
        ImGui::Checkbox("Active", &c.active);
        if (ImGui::Button("Delete")) { entity.Remove<ScriptComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<CameraComponent>() && ImGui::CollapsingHeader("Camera")) {
        ImGui::PushID(9);
        auto& c = entity.Get<CameraComponent>();
        ImGui::DragFloat("FOV", &c.fov, 0.1f);
        ImGui::DragFloat("Pitch", &c.pitch, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<CameraComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<SelectComponent>() && ImGui::CollapsingHeader("Select")) {
        ImGui::PushID(10);
        auto& c = entity.Get<SelectComponent>();
        ImGui::Checkbox("Selected", &c.selected);
        ImGui::Checkbox("Hovered", &c.hovered);
        if (ImGui::Button("Delete")) { entity.Remove<SelectComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<PathComponent>() && ImGui::CollapsingHeader("Path")) {
        ImGui::PushID(11);
        auto& c = entity.Get<PathComponent>();
        ImGui::DragFloat("Speed", &c.speed, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<PathComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<GridComponent>() && ImGui::CollapsingHeader("Grid")) {
        ImGui::PushID(12);
        auto& c = entity.Get<GridComponent>();
        if (ImGui::Button("Delete")) { entity.Remove<GridComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<LightComponent>() && ImGui::CollapsingHeader("Light")) {
        ImGui::PushID(13);
        auto& c = entity.Get<LightComponent>();
        ImGui::ColorPicker3("Colour", &c.colour.r);
        ImGui::DragFloat3("Attenuation", &c.attenuation.x, 0.1f);
        ImGui::DragFloat("Brightness", &c.brightness, 0.1f);
        if (ImGui::Button("Delete")) { entity.Remove<LightComponent>(); }
        ImGui::PopID();
    }
    if (entity.Has<ParticleComponent>() && ImGui::CollapsingHeader("Particle")) {
        ImGui::PushID(14);
        auto& c = entity.Get<ParticleComponent>();
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
 
    ImGui::Separator();

    if (ImGui::BeginMenu("Add Component")) {
        if (!entity.Has<TemporaryComponent>() && ImGui::MenuItem("Temporary")) {
            TemporaryComponent c;
            entity.Add(c);
        }
        if (!entity.Has<NameComponent>() && ImGui::MenuItem("Name")) {
            NameComponent c;
            entity.Add(c);
        }
        if (!entity.Has<TransformComponent>() && ImGui::MenuItem("Transform")) {
            TransformComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ModelComponent>() && ImGui::MenuItem("Model")) {
            ModelComponent c;
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
        if (!entity.Has<ScriptComponent>() && ImGui::MenuItem("Script")) {
            ScriptComponent c;
            entity.Add(c);
        }
        if (!entity.Has<CameraComponent>() && ImGui::MenuItem("Camera")) {
            CameraComponent c;
            entity.Add(c);
        }
        if (!entity.Has<SelectComponent>() && ImGui::MenuItem("Select")) {
            SelectComponent c;
            entity.Add(c);
        }
        if (!entity.Has<PathComponent>() && ImGui::MenuItem("Path")) {
            PathComponent c;
            entity.Add(c);
        }
        if (!entity.Has<GridComponent>() && ImGui::MenuItem("Grid")) {
            GridComponent c;
            entity.Add(c);
        }
        if (!entity.Has<LightComponent>() && ImGui::MenuItem("Light")) {
            LightComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ParticleComponent>() && ImGui::MenuItem("Particle")) {
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
