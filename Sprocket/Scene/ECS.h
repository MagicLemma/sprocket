#pragma once
#include "Types.h"
#include "SparseSet.h"
#include "TypeInfo.h"
#include "Events.h"

#include <unordered_map>
#include <deque>
#include <functional>
#include <limits>
#include <vector>
#include <cassert>
#include <any>
#include <tuple>

#include <cppcoro/generator.hpp>

namespace Sprocket {
namespace ecs {

class Registry;
class Entity;

enum class Identifier : std::uint64_t {};

using Index = std::uint32_t;
using Version = std::uint32_t;

template <typename Comp>
struct Added
{
    ecs::Entity entity;
    Added(ecs::Entity& e) : entity(e) {}
};

template <typename Comp>
struct Removed
{
    ecs::Entity entity;
    Removed(ecs::Entity& e) : entity(e) {}
};

class Entity
{
    Registry*  d_registry;
    Identifier d_identifier;

public:
    Entity(Registry* r, Identifier i);
    Entity();

    bool valid() const;
    void destroy();

    template <typename Comp, typename... Args> Comp& add(Args&&... args);
    template <typename Comp> void remove();
    template <typename Comp> const Comp& get() const;
    template <typename Comp> Comp& get();
    template <typename Comp> bool has() const;

    Identifier id() const;

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
    // Callback that is invoked whenever a component is added or removed from an entity.
    std::function<void(ev::Event&)> d_callback = [](ev::Event&) {};

    // Stores all existing active GUIDs. Allows for index -> guid lookup as well
    // as cache friendly iteration over all entities.
    SparseSet<Identifier> d_entities;

    // When an entity is deleted, their index is added to the pool for reuse.
    std::deque<Identifier> d_pool;

    // Store of all components for all entities. The type of the components are erased.
    struct ComponentData
    {
        // All instances of this component.
        SparseSet<std::any> instances;

        // Returns an ev::Event containing a ComponentRemoveEvent<T>.
        std::function<ev::Event(ecs::Entity)> make_remove_event;
    };

    std::unordered_map<spkt::type_info_t, ComponentData> d_comps;

    bool has(Entity entity, spkt::type_info_t type) const;
    void remove(Entity entity, spkt::type_info_t type);

    Registry& operator=(const Registry&) = delete;
    Registry(const Registry&) = delete;

public:
    Registry() = default;

    // Creates a new entity with no components. This is guaranteed to be a valid handle.
    Entity create();

    // Deletes the given entity along with all of its components.
    void destroy(Entity entity);

    // Returns true if the given entity is currently alive.
    bool valid(Entity entity) const;

    template <typename Comp, typename... Args> Comp& add(Entity entity, Args&&... args);
    template <typename Comp> void remove(Entity entity);
    template <typename Comp> const Comp& get(Entity entity) const;
    template <typename Comp> Comp& get(Entity entity);
    template <typename Comp> bool has(Entity entity) const;

    // Loops through all entities and deletes their components. This will trigger
    // the OnRemove functionality. Callbacks are not removed.
    void clear();

    // Resets all internal storage, removing all entities (without calling OnRemove)
    // and removes all OnAdd/OnRemove callbacks.
    void reset();

    // Number of active entities in the registry.
    std::size_t size() const;

    // Sets the callback that will be invoked whenever a component is added or
    // removed from an entity.
    void set_callback(const std::function<void(ev::Event&)>& callback);

    // Generates all active entities. This is fast, however adding and removing
    // entities while iterating results is undefined.
    cppcoro::generator<Entity> all();

    // Does a fast iteration over all entities with the given Comp. If any extra
    // component types are specified, only entities that have all of those types
    // will be yielded. This should only be used for modifying the components, not
    // adding/removing new ones.
    template <typename Comp, typename... Rest>
    cppcoro::generator<Entity> view();

    // Returns the first entity satisfying the given predicate, or ECS::Null if
    // none is found. Can optionally provide components to filter on.
    template <typename... Comps>
    Entity find(const EntityPredicate& pred = [](Entity){ return true; });

    static std::pair<Index, Version> split(Identifier id);
};

// An "empty" entity.
static const Entity Null{};

// ==============================================================
//                      TEMPLATE DEFINITIONS
// ==============================================================

// REGISTRY TEMPLATES

template <typename Comp, typename... Args>
Comp& Registry::add(Entity entity, Args&&... args)
{
    assert(valid(entity));
    auto [index, version] = Registry::split(entity.id());

    auto& data = d_comps[spkt::type_info<Comp>];

    // If this is the first instance of this component, create the make_remove_event
    // function which will be called whenever an entity is deleted. This is needed because
    // the type of the component is not available at deletion time.
    if (!data.make_remove_event) {
        data.make_remove_event = [](ecs::Entity entity) -> ev::Event {
            return ev::make_event<ecs::Removed<Comp>>(entity);
        };
    }

    auto& entry = data.instances.Insert(
        index, std::make_any<Comp&>(std::forward<Args>(args)...)
    );

    ev::Event event = ev::make_event<Added<Comp>>(entity);
    d_callback(event);

    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
void Registry::remove(Entity entity)
{
    assert(valid(entity));
    remove(entity, spkt::type_info<Comp>);
}

template <typename Comp>
const Comp& Registry::get(Entity entity) const
{
    assert(valid(entity));
    auto [index, version] = Registry::split(entity.id());
    auto& entry = d_comps.at(spkt::type_info<Comp>).instances[index];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
Comp& Registry::get(Entity entity)
{
    assert(valid(entity));
    auto [index, version] = Registry::split(entity.id());
    auto& entry = d_comps.at(spkt::type_info<Comp>).instances[index];
    return std::any_cast<Comp&>(entry);
}

template <typename Comp>
bool Registry::has(Entity entity) const
{
    assert(valid(entity));
    return has(entity, spkt::type_info<Comp>);
}

template <typename Comp, typename... Rest>
cppcoro::generator<Entity> Registry::view()
{
    for (auto& [index, comp] : d_comps[spkt::type_info<Comp>].instances.Fast()) {
        Entity entity{this, d_entities[index]};
        if ((has<Rest>(entity) && ...)) {
            co_yield entity;
        }
    }
}

template <typename... Comps>
Entity Registry::find(const EntityPredicate& pred)
{
    if constexpr (sizeof...(Comps) == 0) {
        for (auto entity : all()) {
            if (pred(entity)) {
                return entity;
            }
        }
    }
    else {
        for (auto entity : view<Comps...>()) {
            if (pred(entity)) {
                return entity;
            }
        }
    }
    return ecs::Null;
}

// ENTITY TEMPLATES

template <typename Comp, typename... Args>
Comp& Entity::add(Args&&... args)
{
    return d_registry->add<Comp>(*this, std::forward<Args>(args)...);
}

template <typename Comp>
void Entity::remove()
{
    d_registry->remove<Comp>(*this);
}

template <typename Comp>
const Comp& Entity::get() const
{
    return d_registry->get<Comp>(*this);
}

template <typename Comp>
Comp& Entity::get()
{
    return d_registry->get<Comp>(*this);
}

template <typename Comp>
bool Entity::has() const
{
    return d_registry->has<Comp>(*this);
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
        return std::hash<Sprocket::ecs::Identifier>{}(entity.id());
    };
};

}