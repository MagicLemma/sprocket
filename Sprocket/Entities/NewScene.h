#pragma once

#include <entt/entt.hpp>

#include <vector>

namespace Sprocket {

class NewScene;

class NewScene
{
    entt::registry d_registry;

public:
    NewScene(); // TODO: Rename and hide EnTT types.

    entt::registry& Registry() { return d_registry; }

    entt::entity CreateEntity();
    void DestroyEntity(entt::entity entity);

    template <typename Component> Component& Add(entt::entity entity);
    template <typename Component> void Remove(entt::entity entity);

    template <typename Component, auto Function> void OnAdd();
    template <typename Component, auto Function, typename Object> void OnAdd(Object& object);
    
    template <typename Component, auto Function> void OnRemove();
    template <typename Component, auto Function, typename Object> void OnRemove(Object& object);
};

template <typename Component> Component& NewScene::Add(entt::entity entity)
{
    return d_registry.assign<Component>(entity);
}

template <typename Component> void NewScene::Remove(entt::entity entity)
{
    d_registry.remove<Component>(entity);
}

template <typename Component, auto Function>
void NewScene::OnAdd()
{
    d_registry.on_construct<Component>().connect<Function>();
}

template <typename Component, auto Function, typename Object>
void NewScene::OnAdd(Object& object)
{
    d_registry.on_construct<Component>().connect<Function>(object);
}

template <typename Component, auto Function>
void NewScene::OnRemove()
{
    d_registry.on_destroy<Component>().connect<Function>();
}

template <typename Component, auto Function, typename Object>
void NewScene::OnRemove(Object& object)
{
    d_registry.on_destroy<Component>().connect<Function>(object);
}

}