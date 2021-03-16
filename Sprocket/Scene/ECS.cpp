#include "ECS.h"
#include "Components.h"

#include <algorithm>
#include <random>

namespace Sprocket {
namespace ecs {

Entity::Entity(Registry* r, std::size_t i, guid::GUID g)
    : d_registry(r)
    , d_index(i)
    , d_guid(g)
{ 
}

Entity::Entity()
    : d_registry(nullptr)
    , d_index(0)
    , d_guid(guid::Zero)
{
}

bool Entity::operator==(Entity other) const
{
    // Change this to only compare registry and guid. If both of these
    // are equal but the index is wrong, we have badly screwed something
    // up, OR have a guid clash. Should assert
    return d_registry == other.d_registry
        && d_index == other.d_index
        && d_guid == other.d_guid;
}

bool Entity::operator!=(Entity other) const
{
    return !(*this == other);
}

Entity& Entity::operator=(Entity other)
{
    d_registry = other.d_registry;
    d_index = other.d_index;
    d_guid = other.d_guid;
    return *this;
}

bool Entity::valid() const
{
     return *this != ecs::Null
         && d_registry
         && d_registry->d_entities.Has(d_index)
         && d_registry->d_entities[d_index] == d_guid;
}

void Entity::destroy() 
{
    if (valid()) {
        // Clean up all components
        for (auto& [type, data] : d_registry->d_comps) {
            if (has(type)) { remove(type); }
        }
        d_registry->d_entities.Erase(d_index);
        d_registry->d_pool.push_back(d_index);
        d_registry->d_lookup.erase(d_guid);
    }
}

guid::GUID Entity::id() const
{
    return d_guid;
}

void Entity::remove(spkt::type_info_t type) const
{
    assert(valid());
    if (!has(type)) { return; }

    auto& data = d_registry->d_comps[type];
    
    ev::Event event = data.make_remove_event(*this);
    d_registry->d_callback(event);

    if (auto it = d_registry->d_comps.find(type); it != d_registry->d_comps.end()) {
        if (it->second.instances.Has(d_index)) {
            it->second.instances.Erase(d_index);
        }
    }
}

bool Entity::has(spkt::type_info_t type) const
{
    if (auto it = d_registry->d_comps.find(type); it != d_registry->d_comps.end()) {
        if (it->second.instances.Has(d_index)) {
            const auto& entry = it->second.instances[d_index];
            return entry.has_value();
        }
    }
    return false;
}

Entity Registry::create()
{
    guid::GUID guid = d_generator.New();
    return create(guid);
}

Entity Registry::create(const guid::GUID& guid)
{
    assert(guid != guid::Zero);

    // If there is a slot in the pool, use that, otherwise increase the
    // the size of the entity sparse set.
    std::size_t index = d_entities.Size();
    if (!d_pool.empty()) {
        index = d_pool.front();
        d_pool.pop_front();
    }

    d_entities.Insert(index, guid);
    d_lookup.emplace(guid, index);
    return {this, index, guid};
}

Entity Registry::get(const guid::GUID& guid)
{
    auto it = d_lookup.find(guid);
    if (it != d_lookup.end()) {
        return Entity{this, it->second, guid};
    }
    return ecs::Null;
}

void Registry::clear()
{
    // Clean up components, triggering on remove behaviour
    for (const auto& [index, guid] : d_entities.Safe()) {
        Entity{this, index, guid}.destroy();
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

void Registry::emit(ev::Event& event)
{
    d_callback(event);
}

cppcoro::generator<Entity> Registry::all()
{
    for (const auto& [index, guid] : d_entities.Fast()) {
        co_yield {this, index, guid};
    }
}

}
}