#include "ECS.h"
#include "Components.h"

#include <algorithm>

namespace Sprocket {
namespace ECS {

bool Entity::operator==(Entity other) const
{
    return d_registry == other.d_registry && d_id == other.d_id;
}

bool Entity::operator!=(Entity other) const
{
    return !(*this == other);
}

Entity& Entity::operator=(Entity other)
{
    d_registry = other.d_registry;
    d_id = other.d_id;
    return *this;
}

bool Entity::Valid() const
{
     return d_registry && d_registry->Valid(d_id);
}

void Entity::Delete() 
{
    d_registry->Delete(d_id);
}

u32 Entity::Id() const
{
    return d_id;
}

u16 Entity::Slot() const
{
    return ECS::Registry::GetSlot(d_id);
}

u16 Entity::Version() const
{
    return ECS::Registry::GetVersion(d_id);
}

Entity Entity::NewEntity() const
{
    return {d_registry, d_registry->New()};
}

Registry::Registry()
    : d_next(0)
{
    d_version.fill(0);
}

void Registry::Remove(u32 entity, std::type_index type)
{
    for (const auto& cb : d_comps[type].onRemove) {
        cb({this, entity});
    }

    if (auto it = d_comps.find(type); it != d_comps.end()) {
        auto& entry = it->second.instances.at(GetSlot(entity));
        entry.reset();
    }
}

u32 Registry::New()
{
    u16 slot = 0;
    if (auto it = d_pool.begin(); it != d_pool.end()) {
        slot = *it;
        d_pool.erase(it);
    }
    else {
        slot = d_next++;
    }
    d_entities[slot] = true;
    u16 version = ++d_version[slot];
    return GetID(slot, version);
}

void Registry::Delete(u32 entity)
{
    // Clean up all components
    for (auto& [type, data] : d_comps) {
        Remove(entity, type);
    }

    // Remove the entity slot from the "alive" list
    d_entities[GetSlot(entity)] = false;

    // Add the entity slot to the pool of available IDs.
    d_pool.insert(GetSlot(entity));
}

bool Registry::Valid(u32 entity) const
{
    u16 slot = GetSlot(entity);
    u16 version = GetVersion(entity);
    return entity != NULL_ID
        && slot < d_next
        && !d_pool.contains(slot)
        && version == d_version[slot];
}

std::size_t Registry::Size() const
{
    return d_next - d_pool.size();
}

u32 Registry::GetID(u16 index, u16 version)
{
    return (u32)version << 16 | index;
}

u16 Registry::GetSlot(u32 id)
{
    return static_cast<u16>(id);
}

u16 Registry::GetVersion(u32 id)
{
    return static_cast<u16>(id >> 16);
}

Registry::Iterator& Registry::Iterator::operator++()
{
    ++d_index;

    // Iterate up to the next alive entity
    while (d_index != d_reg->d_next && d_reg->d_entities[d_index] == false) ++d_index;

    return *this;
}

Entity Registry::Iterator::operator*()
{
    return {d_reg, GetID(d_index, d_reg->d_version[d_index])};
}

bool Registry::Iterator::operator==(const Iterator& other) const
{
    return d_reg == other.d_reg && d_index == other.d_index;
}

bool Registry::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

}
}