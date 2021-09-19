#include "Inspector.h"

#include <Anvil/Anvil.h>

#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/Loader.h>
#include <Sprocket/Scene/meta.h>
#include <Sprocket/UI/DevUI.h>
#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Utility/Maths.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace {

template <typename T>
struct inspector_display;

template <>
struct inspector_display<spkt::Runtime>
{
    static void draw(Anvil& editor, spkt::Runtime& c)
    {
    }
};

template <>
struct inspector_display<spkt::Singleton>
{
    static void draw(Anvil& editor, spkt::Singleton& c)
    {
    }
};

template <>
struct inspector_display<spkt::Event>
{
    static void draw(Anvil& editor, spkt::Event& c)
    {
    }
};

template <>
struct inspector_display<spkt::NameComponent>
{
    static void draw(Anvil& editor, spkt::NameComponent& c)
    {
        spkt::ImGuiXtra::TextModifiable(c.name);
    }
};

template <>
struct inspector_display<spkt::Transform2DComponent>
{
    static void draw(Anvil& editor, spkt::Transform2DComponent& c)
    {
        ImGui::DragFloat2("Position", &c.position.x, 0.1f);
        ImGui::DragFloat("Rotation", &c.rotation, 0.01f);
        ImGui::DragFloat2("Scale", &c.scale.x, 0.1f);
    }
};

template <>
struct inspector_display<spkt::Transform3DComponent>
{
    static void draw(Anvil& editor, spkt::Transform3DComponent& c)
    {
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
    }
};

template <>
struct inspector_display<spkt::StaticModelComponent>
{
    static void draw(Anvil& editor, spkt::StaticModelComponent& c)
    {
        spkt::ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
        spkt::ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
    }
};

template <>
struct inspector_display<spkt::AnimatedModelComponent>
{
    static void draw(Anvil& editor, spkt::AnimatedModelComponent& c)
    {
        spkt::ImGuiXtra::File("Mesh", editor.window(), &c.mesh, "*.obj");
        spkt::ImGuiXtra::File("Material", editor.window(), &c.material, "*.yaml");
        spkt::ImGuiXtra::TextModifiable(c.animation_name);
        ImGui::DragFloat("Animation Time", &c.animation_time, 0.01f);
        ImGui::DragFloat("Animation Speed", &c.animation_speed, 0.01f);
    }
};

template <>
struct inspector_display<spkt::RigidBody3DComponent>
{
    static void draw(Anvil& editor, spkt::RigidBody3DComponent& c)
    {
        ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
        ImGui::Checkbox("Gravity", &c.gravity);
        ImGui::Checkbox("Frozen", &c.frozen);
        ImGui::SliderFloat("Bounciness", &c.bounciness, 0.0f, 1.0f);
        ImGui::SliderFloat("Friction Coefficient", &c.frictionCoefficient, 0.0f, 1.0f);
        ImGui::SliderFloat("Rolling Resistance", &c.rollingResistance, 0.0f, 1.0f);
        ImGui::DragFloat3("Force", &c.force.x, 0.1f);
        ImGui::Checkbox("OnFloor", &c.onFloor);
        ;
    }
};

template <>
struct inspector_display<spkt::BoxCollider3DComponent>
{
    static void draw(Anvil& editor, spkt::BoxCollider3DComponent& c)
    {
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.01f);
        ImGui::DragFloat3("Half Extents", &c.halfExtents.x, 0.1f);
        ImGui::Checkbox("Apply Scale", &c.applyScale);
        ;
    }
};

template <>
struct inspector_display<spkt::SphereCollider3DComponent>
{
    static void draw(Anvil& editor, spkt::SphereCollider3DComponent& c)
    {
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.01f);
        ImGui::DragFloat("Radius", &c.radius, 0.01f);
        ;
    }
};

template <>
struct inspector_display<spkt::CapsuleCollider3DComponent>
{
    static void draw(Anvil& editor, spkt::CapsuleCollider3DComponent& c)
    {
        ImGui::DragFloat3("Position", &c.position.x, 0.1f);
        spkt::ImGuiXtra::Euler("Orientation", &c.orientation);
        ImGui::DragFloat("Mass", &c.mass, 0.01f);
        ImGui::DragFloat("Radius", &c.radius, 0.01f);
        ImGui::DragFloat("Height", &c.height, 0.01f);
        ;
    }
};

template <>
struct inspector_display<spkt::ScriptComponent>
{
    static void draw(Anvil& editor, spkt::ScriptComponent& c)
    {
        spkt::ImGuiXtra::File("Script", editor.window(), &c.script, "*.lua");
        ImGui::Checkbox("Active", &c.active);
        ;
    }
};

template <>
struct inspector_display<spkt::Camera3DComponent>
{
    static void draw(Anvil& editor, spkt::Camera3DComponent& c)
    {
        ;
        ImGui::DragFloat("FOV", &c.fov, 0.01f);
        ImGui::DragFloat("Pitch", &c.pitch, 0.01f);
    }
};

template <>
struct inspector_display<spkt::PathComponent>
{
    static void draw(Anvil& editor, spkt::PathComponent& c)
    {
        ;
        ImGui::DragFloat("Speed", &c.speed, 0.01f);
    }
};

template <>
struct inspector_display<spkt::LightComponent>
{
    static void draw(Anvil& editor, spkt::LightComponent& c)
    {
        ImGui::ColorEdit3("Colour", &c.colour.r);
        ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
    }
};

template <>
struct inspector_display<spkt::SunComponent>
{
    static void draw(Anvil& editor, spkt::SunComponent& c)
    {
        ImGui::ColorEdit3("Colour", &c.colour.r);
        ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
        ImGui::DragFloat3("Direction", &c.direction.x, 0.1f);
        ImGui::Checkbox("Shadows", &c.shadows);
    }
};

template <>
struct inspector_display<spkt::AmbienceComponent>
{
    static void draw(Anvil& editor, spkt::AmbienceComponent& c)
    {
        ImGui::ColorEdit3("Colour", &c.colour.r);
        ImGui::DragFloat("Brightness", &c.brightness, 0.01f);
    }
};

template <>
struct inspector_display<spkt::ParticleComponent>
{
    static void draw(Anvil& editor, spkt::ParticleComponent& c)
    {
        ImGui::DragFloat("Interval", &c.interval, 0.01f);
        ImGui::DragFloat3("Velocity", &c.velocity.x, 0.1f);
        ImGui::DragFloat("Velocity Noise", &c.velocityNoise, 0.01f);
        ImGui::DragFloat3("Acceleration", &c.acceleration.x, 0.1f);
        ImGui::DragFloat3("Scale", &c.scale.x, 0.1f);
        ImGui::DragFloat("Life", &c.life, 0.01f);
        ImGui::DragFloat("Accumulator", &c.accumulator, 0.01f);
    }
};

template <>
struct inspector_display<spkt::CollisionEvent>
{
    static void draw(Anvil& editor, spkt::CollisionEvent& c)
    {
        ;
        ;
    }
};

template <>
struct inspector_display<spkt::PhysicsSingleton>
{
    static void draw(Anvil& editor, spkt::PhysicsSingleton& c)
    {
        ;
    }
};

template <>
struct inspector_display<spkt::InputSingleton>
{
    static void draw(Anvil& editor, spkt::InputSingleton& c)
    {
        ;
    }
};

template <>
struct inspector_display<spkt::GameGridSingleton>
{
    static void draw(Anvil& editor, spkt::GameGridSingleton& c)
    {
        ;
        ;
        ;
        ;
    }
};

template <>
struct inspector_display<spkt::TileMapSingleton>
{
    static void draw(Anvil& editor, spkt::TileMapSingleton& c)
    {
        ;
    }
};

template <>
struct inspector_display<spkt::CameraSingleton>
{
    static void draw(Anvil& editor, spkt::CameraSingleton& c)
    {
        ;
    }
};

template <>
struct inspector_display<spkt::ParticleSingleton>
{
    static void draw(Anvil& editor, spkt::ParticleSingleton& c)
    {
        ;
        ;
    }
};


}

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
    spkt::for_each_reflect([&]<typename T>(spkt::reflection<T> refl) {
        if (registry.has<T>(entity)) {
            auto& c = registry.get<T>(entity);
            if (ImGui::CollapsingHeader(refl.name)) {
                ImGui::PushID(count++);
                inspector_display<T>::draw(editor, c);
                if constexpr (std::is_same_v<T, spkt::Transform3DComponent>) {
                    spkt::ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
                }
                if (ImGui::Button("Delete")) { registry.remove<T>(entity); }
                ImGui::PopID();
            }
        }
    });

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        spkt::for_each_reflect([&]<typename T>(spkt::reflection<T> refl) {
            if (!registry.has<T>(entity) && ImGui::Selectable(refl.name)) {
                registry.add<T>(entity, {});
            }
        });
        ImGui::EndMenu();
    }
    ImGui::Separator();

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
