#pragma once
#include "Types.h"
#include "SparseSet.h"
#include "Log.h"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include <array>
#include <memory>
#include <queue>
#include <functional>
#include <deque>
#include <limits>
#include <vector>
#include <cassert>
#include <any>

#include <cppcoro/generator.hpp>

namespace Sprocket {
namespace ECS {
    
class Registry;

static constexpr u32 NULL_ID = std::numeric_limits<u32>::max();

class Entity
{
    ECS::Registry* d_registry;
    u32            d_id;

public:
    Entity(ECS::Registry* r, u32 i) : d_registry(r), d_id(i) {}
    Entity() : d_registry(nullptr), d_id(NULL_ID) {}

    bool Valid() const;
    void Delete();

    template <typename Comp> Comp& Add();
    template <typename Comp> Comp& Add(const Comp& component);
    template <typename Comp, typename... Args> Comp& Emplace(Args&&... args);
    template <typename Comp> void Remove();
    template <typename Comp> Comp& Get();
    template <typename Comp> const Comp& Get() const;
    template <typename Comp> bool Has() const;

    u32 Id() const;
    u16 Slot() const;
    u16 Version() const;

    bool operator==(Entity other) const;
    bool operator!=(Entity other) const;
    Entity& operator=(Entity other);

    // TODO: Remove
    Entity NewEntity() const;
};

class Registry
{
public:
    using EntityCallback = std::function<void(Entity)>;
    using EntityPredicate = std::function<bool(Entity)>;

private:
    // Stores the current version of each entity.
    SparseSet<u16> d_entities;

    // When an entity is removed, their slot/version is added to the pool so that it
    // can be reused.
    std::queue<std::pair<u16, u16>> d_pool;

    // Store of all components for all entities. The type of the components are erased.
    struct ComponentData
    {
        // All instances of this component.
        SparseSet<std::any> instances;

        // Callbacks triggered whenever a new instance of this component is added. These
        // are called after the component has been added.
        std::vector<EntityCallback> onAdd;

        // Callbacks triggers whenever an instance of this component is remove. These
        // are called before the component has been removed.
        std::vector<EntityCallback> onRemove;
    };

    std::unordered_map<std::type_index, ComponentData> d_comps;

    void Remove(u32 entity, std::type_index type);

public:
    Registry() = default;

    Entity New();
    void Delete(Entity entity);
    void Delete(const std::vector<Entity>& entities);

    // Loops through all entities and deletes their components. This will trigger
    // the OnRemove functionality.
    void Clear();

    // Number of active entities in the registry.
    std::size_t Size() const;

    // Returns the first entity satisfying the given predicate, or ECS::Null if
    // none is found.
    Entity Find(const EntityPredicate& pred);

    // Generates all active entities. This is fast, however adding and removing
    // entities while iterating results is undefined.
    cppcoro::generator<Entity> Fast();

    // Generates all active entities. This is slow, but adding and removing
    // entities is safe. If an entity is added, it may or may not be included
    // in the iteration, depending on where in the sparse set it gets added.
    cppcoro::generator<Entity> Safe();

    // Does a fast iteration over all entities with the given component.
    // This should only be used for modifying the components, not adding/removing
    // new ones.
    template <typename Comp, typename... Rest> cppcoro::generator<Entity> View();

    template <typename Comp> void OnAdd(const EntityCallback& cb);
    template <typename Comp> void OnRemove(const EntityCallback& cb);

    bool Valid(u32 entity) const;

    template <typename Comp> Comp& Add(u32 entity);
    template <typename Comp> Comp& Add(u32 entity, const Comp& component);
    template <typename Comp, typename... Args> Comp& Emplace(u32 entity, Args&&... args);
    template <typename Comp> void Remove(u32 entity);
    template <typename Comp> Comp& Get(u32 entity);
    template <typename Comp> const Comp& Get(u32 entity) const;
    template <typename Comp> bool Has(u32 entity) const;

    static u32 GetID(u16 slot, u16 version);
    static u16 GetSlot(u32 id);
    static u16 GetVersion(u32 id);
};

// An "empty" entity.
static const Entity Null(nullptr, NULL_ID);

// ==============================================================
//                      TEMPLATE DEFINITIONS
// ==============================================================

// REGISTRY TEMPLATES

template <typename Comp>
Comp& Registry::Add(u32 entity)
{
    return Add<Comp>(entity, Comp{});
}

template <typename Comp>
Comp& Registry::Add(u32 entity, const Comp& component)
{
    assert(Valid(entity));
    auto& entry = d_comps[typeid(Comp)].instances[GetSlot(entity)];
    entry = component;

    for (const auto& cb : d_comps[typeid(Comp)].onAdd) {
        cb({this, entity});
    }

    return std::any_cast<Comp&>(entry);
}

template <typename Comp, typename... Args>
Comp& Registry::Emplace(u32 entity, Args&&... args)
{
    assert(Valid(entity));
    auto& entry = d_comps[typeid(Comp)].instances[GetSlot(entity)];
    entry = std::make_any<Comp&>(std::forward<Args>(args)...);

    for (const auto& cb : d_comps[typeid(Comp)].onAdd) {
        cb({this, entity});
    }

    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
void Registry::Remove(u32 entity)
{
    assert(Valid(entity));
    Remove(entity, typeid(Comp));
}

template <typename Comp>
Comp& Registry::Get(u32 entity)
{
    assert(Valid(entity));
    auto& entry = d_comps.at(typeid(Comp)).instances[GetSlot(entity)];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
const Comp& Registry::Get(u32 entity) const
{
    assert(Valid(entity));
    auto& entry = d_comps.at(typeid(Comp)).instances[GetSlot(entity)];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
bool Registry::Has(u32 entity) const
{
    assert(Valid(entity));
    if (auto it = d_comps.find(typeid(Comp)); it != d_comps.end()) {
        if (it->second.instances.Has(GetSlot(entity))) {
            const auto& entry = it->second.instances[GetSlot(entity)];
            return entry.has_value();
        }
    }
    return false;
}

template <typename Comp>
void Registry::OnAdd(const EntityCallback& cb)
{
    d_comps[typeid(Comp)].onAdd.push_back(cb);
}

template <typename Comp>
void Registry::OnRemove(const EntityCallback& cb)
{
    d_comps[typeid(Comp)].onRemove.push_back(cb);
}

template <typename Comp, typename... Rest>
cppcoro::generator<Entity> Registry::View()
{
    for (auto& [index, comp] : d_comps[typeid(Comp)].instances.Fast()) {
        Entity entity{this, GetID(index, d_entities[index])};
        if ((entity.Has<Rest>() && ...)) {
            co_yield entity;
        }
    }
}

// ENTITY TEMPLATES

template <typename Comp>
Comp& Entity::Add()
{
    return Add<Comp>(Comp{});
}

template <typename Comp>
Comp& Entity::Add(const Comp& component)
{
    assert(Valid());
    return d_registry->Add<Comp>(d_id, component);
}

template <typename Comp, typename... Args>
Comp& Entity::Emplace(Args&&... args)
{
    assert(Valid());
    return d_registry->Emplace<Comp>(d_id, std::forward<Args>(args)...);
}

template <typename Comp>
void Entity::Remove()
{
    assert(Valid());
    d_registry->Remove<Comp>(d_id);
}

template <typename Comp>
Comp& Entity::Get()
{
    assert(Valid());
    return d_registry->Get<Comp>(d_id);
}

template <typename Comp>
const Comp& Entity::Get() const
{
    assert(Valid());
    return d_registry->Get<Comp>(d_id);
}

template <typename Comp>
bool Entity::Has() const
{
    assert(Valid());
    return d_registry->Has<Comp>(d_id);
}

}
}