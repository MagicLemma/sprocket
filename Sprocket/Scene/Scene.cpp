#include "Scene.h"
#include "Components.h"
#include "Loader.h"
#include "LambdaSystem.h"

namespace spkt {

Scene::Scene(Window* window)
    : d_window(window)
{
    auto singleton = d_registry.create();
    d_registry.emplace<Singleton>(singleton);
    d_registry.emplace<TemporaryComponent>(singleton);
    d_registry.emplace<NameComponent>(singleton, "::RuntimeSingleton");
    d_registry.emplace<InputSingleton>(singleton);
}

Scene::~Scene()
{
    d_registry.clear();
}

void Scene::add(const std::function<void(spkt::registry&, double)>& system)
{
    Add<LambdaSystem>(system);
}

void Scene::Load(std::string_view file)
{
    Loader::Load(std::string(file), &Entities());
}

void Scene::OnUpdate(double dt)
{
    for (auto& system : d_systems) {
        system->on_update(d_registry, dt);
    }

    auto singleton = d_registry.find<Singleton>();
    if (d_registry.valid(singleton)) {
        auto& input = d_registry.get<InputSingleton>(singleton);
        input.mouse_click.fill(false);
        input.mouse_unclick.fill(false);
        input.mouse_offset = {0.0, 0.0};
        input.mouse_scrolled = {0.0, 0.0};
    }
}

void Scene::OnEvent(ev::Event& event)
{
    auto singleton = d_registry.find<Singleton>();
    if (!d_registry.valid(singleton)) {
        return;
    }

    auto& input = d_registry.get<InputSingleton>(singleton);

    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (!event.is_consumed()) {
            input.keyboard[data->key] = true;
        }
    }
    else if (auto data = event.get_if<ev::KeyboardButtonReleased>()) {
        input.keyboard[data->key] = false;
    }
    else if (auto data = event.get_if<ev::MouseButtonPressed>()) {
        if (!event.is_consumed()) { 
            input.mouse[data->button] = true;
            input.mouse_click[data->button] = true;
        }
    }
    else if (auto data = event.get_if<ev::MouseButtonReleased>()) {
        input.mouse[data->button] = false;
        input.mouse_unclick[data->button] = true;
    }
    else if (auto data = event.get_if<ev::MouseScrolled>()) {
        input.mouse_scrolled.x += data->x_offset;
        input.mouse_scrolled.y += data->y_offset;
    }
    else if (auto data = event.get_if<ev::WindowResize>()) {
        input.window_resized = true;
    }

    input.mouse_pos = d_window->GetMousePos();
    input.mouse_offset = d_window->GetMouseOffset();

    input.window_width = (float)d_window->Width();
    input.window_height = (float)d_window->Height();
}

std::size_t Scene::Size() const
{
    return d_registry.size();
}

}
