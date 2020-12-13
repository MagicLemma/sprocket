#include "ECS.h"
#include "Components.h"

#include <algorithm>

namespace Sprocket {

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

namespace ECS {

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
        d_entities.push_back(slot);
    }
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
    auto it = std::find(d_entities.begin(), d_entities.end(), GetSlot(entity));
    d_entities.erase(it);

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

void Registry::Sort(const Comparitor& compare)
{
    const auto c = [&](u16 a, u16 b) {
        return compare(
            {this, GetID(a, d_version[a])},
            {this, GetID(b, d_version[b])}
        );
    };
    std::sort(d_entities.begin(), d_entities.end(), c);
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
    ++d_dequeIndex;
    return *this;
}

Entity Registry::Iterator::operator*()
{
    u16 index = d_reg->d_entities[d_dequeIndex];
    u16 version = d_reg->d_version[index];
    return {d_reg, GetID(index, version)};
}

bool Registry::Iterator::operator==(const Iterator& other) const
{
    return d_reg == other.d_reg && d_dequeIndex == other.d_dequeIndex;
}

bool Registry::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

}
}