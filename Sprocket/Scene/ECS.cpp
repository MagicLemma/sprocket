#include "ECS.h"
#include "Components.h"

#include <cpp-itertools.hpp>

#include <algorithm>

namespace Sprocket {
namespace ecs {

bool Entity::operator==(Entity other) const
{
    return d_registry == other.d_registry
        && d_index == other.d_index
        && d_version == other.d_version;
}

bool Entity::operator!=(Entity other) const
{
    return !(*this == other);
}

Entity& Entity::operator=(Entity other)
{
    d_registry = other.d_registry;
    d_index = other.d_index;
    d_version = other.d_version;
    return *this;
}

bool Entity::Valid() const
{
     return *this != ecs::Null
         && d_registry
         && d_registry->d_entities.Has(d_index)
         && d_registry->d_entities[d_index] == d_version;
}

void Entity::Delete() 
{
    if (Valid()) {
        // Clean up all components
        for (auto& [type, data] : d_registry->d_comps) {
            if (Has(type)) { Remove(type); }
        }
        d_registry->d_entities.Erase(d_index);

        // Add the entity slot to the pool of available IDs.
        d_registry->d_pool.push_back({d_index, d_version});
    }
}

u32 Entity::Id() const
{
    return (u32)d_version << 16 | d_index;
}

u16 Entity::Index() const
{
    return d_index;
}

u16 Entity::Version() const
{
    return d_version;
}

void Entity::Remove(std::type_index type)
{
    assert(Valid());
    if (!Has(type)) { return; }
    
    for (const auto& cb : d_registry->d_comps[type].onRemove) {
        cb(*this);
    }

    if (auto it = d_registry->d_comps.find(type); it != d_registry->d_comps.end()) {
        if (it->second.instances.Has(d_index)) {
            it->second.instances.Erase(d_index);
        }
    }
}

bool Entity::Has(std::type_index type) const
{
    if (auto it = d_registry->d_comps.find(type); it != d_registry->d_comps.end()) {
        if (it->second.instances.Has(d_index)) {
            const auto& entry = it->second.instances[d_index];
            return entry.has_value();
        }
    }
    return false;
}

Entity Registry::New()
{
    u16 index = 0;
    u16 version = 0;

    // If there is a slot in the pool, pop it and bump its version.
    if (!d_pool.empty()) {
        std::tie(index, version) = d_pool.front();
        d_pool.pop_front();
        ++version;
    }
    // Otherwise we append on the end.
    else {
        index = d_entities.Size(); // size of sparse == size of packed here
    }

    d_entities.Insert(index, version);
    return {this, index, version};
}

Entity Registry::Get(u32 id)
{
    u16 index = (u16)id;
    u16 version = (u32)(id >> 16);
    return {this, index, version};
}

void Registry::DeleteAll()
{
    // Clean up components, triggering onRemove behaviour
    for (const auto& [index, version] : d_entities.Safe()) {
        Entity{this, index, version}.Delete();
    }

    // Reset all entity storage
    d_entities.Clear();
    d_pool.clear();

    // TODO: Also reset component storage without affecting OnAdd/OnRemove callbacks
}

void Registry::Clear()
{
    // Reset all components and remove all callbacks
    d_comps.clear();

    // Reset all entity storage
    d_entities.Clear();
    d_pool.clear();
}

std::size_t Registry::Size() const
{
    return d_entities.Size();
}

cppcoro::generator<Entity> Registry::Each()
{
    for (const auto& [index, version] : d_entities.Fast()) {
        co_yield {this, index, version};
    }
}

Registry::Registry(const std::vector<Slot>& slots)
{
    for (const auto& [indexull, slot] : itertools::enumerate(slots)) {
        u16 index = static_cast<u16>(indexull);
        const auto& [version, active] = slot;

        if (active) {
            d_entities.Insert(index, version);
        } else {
            d_pool.push_back({index, version});
        }
    }
}

std::vector<Registry::Slot> Registry::SlotInfo() const
{
    std::vector<Registry::Slot> slots;
    slots.resize(d_entities.Size() + d_pool.size());

    // Alive entities
    for (const auto& [index, version] : d_entities.Fast()) {
        slots[index] = {version, true};
    }

    // Dead entities
    for (const auto& [index, version] : d_pool) {
        slots[index] = {version, false};
    }

    return slots;
}

}
}