#pragma once
#include "Types.h"
#include "SparseSet.h"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <queue>
#include <functional>
#include <limits>
#include <vector>
#include <cassert>
#include <any>

#include <cppcoro/generator.hpp>

namespace Sprocket {
namespace ecs {
    
class Registry;

static constexpr u32 NULL_ID = std::numeric_limits<u32>::max();
static constexpr u16 NULL_INDEX = std::numeric_limits<u32>::max();
static constexpr u16 NULL_VERSION = std::numeric_limits<u32>::max();

class Entity
{
    Registry* d_registry;
    u16       d_index;
    u16       d_version;

    bool Has(std::type_index type) const;
    void Remove(std::type_index type);

public:
    Entity(Registry* r, u16 i, u16 v) : d_registry(r), d_index(i), d_version(v) {}
    Entity() : d_registry(nullptr), d_index(NULL_INDEX), d_version(NULL_VERSION) {}

    bool Valid() const;
    void Delete();

    template <typename Comp, typename... Args> Comp& Add(Args&&... args);
    template <typename Comp> void Remove();
    template <typename Comp> Comp& Get();
    template <typename Comp> const Comp& Get() const;
    template <typename Comp> bool Has() const;

    u32 Id() const;

    bool operator==(Entity other) const;
    bool operator!=(Entity other) const;
    Entity& operator=(Entity other);
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

public:
    Registry() = default;

    // Creates a new entity with no components. This is guaranteed to be a valid handle.
    Entity New();

    // Given an entity ID, return the entity handle associated to it.
    Entity Get(u32 id); 

    // Loops through all entities and deletes their components. This will trigger
    // the OnRemove functionality. Callbacks are not removed.
    void DeleteAll();

    // Resets all internal storage, removing all entities (without calling OnRemove)
    // and removes all OnAdd/OnRemove callbacks.
    void Clear();

    // Number of active entities in the registry.
    std::size_t Size() const;

    // Generates all active entities. This is fast, however adding and removing
    // entities while iterating results is undefined.
    cppcoro::generator<Entity> Each();

    // Does a fast iteration over all entities with the given Comp. If any extra
    // component types are specified, only entities that have all of those types
    // will be yielded. This should only be used for modifying the components, not
    // adding/removing new ones.
    template <typename Comp, typename... Rest> cppcoro::generator<Entity> View();

    // Returns the first entity satisfying the given predicate, or ECS::Null if
    // none is found. Can optionally provide components to filter on.
    template <typename... Comps> Entity Find(const EntityPredicate& pred = [](Entity){ return true; });

    // Registers a function that will be called whenever a Comp is added to an
    // entity. This is called after the component has been added.
    template <typename Comp> void OnAdd(const EntityCallback& cb);

    // Registers a function that will be called whenever a Comp is removed from
    // an entity. This is called before the compoent has been removed.
    template <typename Comp> void OnRemove(const EntityCallback& cb);

    friend class Entity;
};

// An "empty" entity.
static const Entity Null(nullptr, NULL_INDEX, NULL_VERSION);

// ==============================================================
//                      TEMPLATE DEFINITIONS
// ==============================================================

// REGISTRY TEMPLATES

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
        Entity entity{this, index, d_entities[index]};
        if ((entity.Has<Rest>() && ...)) {
            co_yield entity;
        }
    }
}

template <typename... Comps>
Entity Registry::Find(const EntityPredicate& pred)
{
    if constexpr (sizeof...(Comps) == 0) {
        for (auto entity : Each()) {
            if (pred(entity)) {
                return entity;
            }
        }
    }
    else {
        for (auto entity : View<Comps...>()) {
            if (pred(entity)) {
                return entity;
            }
        }
    }
    return ecs::Null;
}

// ENTITY TEMPLATES

template <typename Comp, typename... Args>
Comp& Entity::Add(Args&&... args)
{
    assert(Valid());
    auto& data = d_registry->d_comps[typeid(Comp)];
    auto& entry = data.instances.Insert(
        d_index, std::make_any<Comp&>(std::forward<Args>(args)...)
    );

    for (const auto& cb : data.onAdd) {
        cb(*this);
    }

    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
void Entity::Remove()
{
    assert(Valid());
    Remove(typeid(Comp));
}

template <typename Comp>
Comp& Entity::Get()
{
    assert(Valid());
    auto& entry = d_registry->d_comps.at(typeid(Comp)).instances[d_index];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
const Comp& Entity::Get() const
{
    assert(Valid());
    auto& entry = d_registry->d_comps.at(typeid(Comp)).instances[d_index];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
bool Entity::Has() const
{
    assert(Valid());
    return Has(typeid(Comp));
}

// We can push an entity into the Lua stack by calling the Lua equivalent of malloc
// and setting that memory to an entity. For this, we need Entity to be copy
// assignable. As this will get deleted by the garbage collector, we also require that
// Entity be trivially destrucible so there are no side effects that we would miss.
static_assert(std::is_trivially_destructible<Entity>());

static_assert(std::is_copy_assignable<Entity>());
static_assert(std::is_copy_constructible<Entity>());

static_assert(std::is_move_assignable<Entity>());
static_assert(std::is_move_constructible<Entity>());

}
}

namespace std {

template <> struct hash<Sprocket::ecs::Entity>
{
    std::size_t operator()(const Sprocket::ecs::Entity& entity) const noexcept
    {
        return std::hash<std::uint32_t>{}(entity.Id());
    };
};

}