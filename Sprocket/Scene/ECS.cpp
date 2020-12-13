#include "ECS.h"
#include "Components.h"

#include <algorithm>

namespace Sprocket {
namespace ECS {

bool Entity::operator==(Entity other) const
{
    return registry == other.registry && id == other.id;
}

bool Entity::operator!=(Entity other) const
{
    return !(*this == other);
}

bool Entity::Valid() const
{
     return registry && registry->Valid(id);
}

void Entity::Delete() 
{
    registry->Delete(*this);
}

u16 Entity::Slot() const
{
    return ECS::Registry::GetSlot(id);
}

u16 Entity::Version() const
{
    return ECS::Registry::GetVersion(id);
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

Entity Registry::New()
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
    return {this, GetID(slot, version)};
}

void Registry::Delete(Entity entity)
{
    // Clean up all components
    for (auto& [type, data] : d_comps) {
        Remove(entity.id, type);
    }

    // Remove the entity slot from the "alive" list
    auto it = std::find(d_entities.begin(), d_entities.end(), entity.Slot());
    d_entities.erase(it);

    // Add the entity slot to the pool of available IDs.
    d_pool.insert(entity.Slot());
}

bool Registry::Valid(u32 entity) const
{
    u16 slot = GetSlot(entity);
    u16 version = GetVersion(entity);
    return entity != ECS::NULL_ID
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