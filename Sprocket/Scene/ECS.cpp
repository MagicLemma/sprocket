#include "ECS.h"
#include "Components.h"

#include <algorithm>
#include <random>

namespace Sprocket {
namespace ecs {
namespace {

static constexpr Identifier null_id = std::numeric_limits<Identifier>::max();

Identifier combine(Index i, Version v)
{
    using Int = std::underlying_type_t<Identifier>;
    return static_cast<Identifier>(((Int)i << 32) + (Int)v);
}

}

Entity::Entity(Registry* r, Identifier i)
    : d_registry(r)
    , d_identifier(i)
{ 
}

Entity::Entity()
    : d_registry(nullptr)
    , d_identifier(null_id)
{
}

bool Entity::operator==(Entity other) const
{
    // Change this to only compare registry and guid. If both of these
    // are equal but the index is wrong, we have badly screwed something
    // up, OR have a guid clash. Should assert
    return d_registry == other.d_registry
        && d_identifier == other.d_identifier;
}

bool Entity::operator!=(Entity other) const
{
    return !(*this == other);
}

Entity& Entity::operator=(Entity other)
{
    d_registry = other.d_registry;
    d_identifier = other.d_identifier;
    return *this;
}

bool Entity::valid() const
{
    auto [index, version] = Registry::split(d_identifier);
    return *this != ecs::Null
        && d_registry
        && d_registry->d_entities.Has(index)
        && d_registry->d_entities[index] == d_identifier;
}

void Entity::destroy() 
{
    d_registry->destroy(*this);
}

Identifier Entity::id() const
{
    return d_identifier;
}

void Registry::remove(Entity entity, spkt::type_info_t type)
{
    assert(entity.valid());
    if (!has(entity, type)) { return; }

    auto& data = d_comps[type];
    
    ev::Event event = data.make_remove_event(entity);
    d_callback(event);

    auto [index, version] = Registry::split(entity.id());
    if (auto it = d_comps.find(type); it != d_comps.end()) {
        if (it->second.instances.Has(index)) {
            it->second.instances.Erase(index);
        }
    }
}

bool Registry::has(Entity entity, spkt::type_info_t type) const
{
    auto [index, version] = Registry::split(entity.id());
    if (auto it = d_comps.find(type); it != d_comps.end()) {
        if (it->second.instances.Has(index)) {
            const auto& entry = it->second.instances[index];
            return entry.has_value();
        }
    }
    return false;
}

Entity Registry::create()
{
    Index index = d_entities.Size();
    Version version = 0;
    if (!d_pool.empty()) {
        std::tie(index, version) = split(d_pool.front());
        d_pool.pop_front();
        ++version;
    }

    Identifier id = combine(index, version);
    d_entities.Insert(index, id);
    return {this, id};
}

void Registry::destroy(Entity entity)
{
    assert(entity.valid());
    auto [index, version] = Registry::split(entity.id());

    // Clean up all components
    for (auto& [type, data] : d_comps) {
        if (has(entity, type)) { remove(entity, type); }
    }
    d_entities.Erase(index);
    d_pool.push_back(entity.id());
}

Entity Registry::get(Identifier id)
{
    return {this, id};
}

void Registry::clear()
{
    // Clean up components, triggering on remove behaviour
    for (const auto& [index, id] : d_entities.Safe()) {
        Entity{this, id}.destroy();
    }

    // Reset all entity storage
    d_entities.Clear();
    d_pool.clear();
}

void Registry::reset()
{
    // Reset all components and remove all callbacks
    d_comps.clear();

    // Reset all entity storage
    d_entities.Clear();
    d_pool.clear();
}

std::size_t Registry::size() const
{
    return d_entities.Size();
}

void Registry::set_callback(const std::function<void(ev::Event&)>& callback)
{
    d_callback = callback;
}

cppcoro::generator<Entity> Registry::all()
{
    for (const auto& [index, id] : d_entities.Fast()) {
        co_yield {this, id};
    }
}

std::pair<Index, Version> Registry::split(Identifier id)
{
    using Int = std::underlying_type_t<Identifier>;
    Int id_int = static_cast<Int>(id);
    return {(Index)(id_int >> 32), (Version)id_int};
}

}
}