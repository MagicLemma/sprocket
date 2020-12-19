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
    d_registry->Delete(*this);
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
    return d_registry->New();
}

Registry::Registry()
    : d_next(0)
{
    d_entities.fill(NULL_ID);
}

void Registry::Remove(u32 entity, std::type_index type)
{
    for (const auto& cb : d_comps[type].onRemove) {
        cb({this, entity});
    }

    if (auto it = d_comps.find(type); it != d_comps.end()) {
        if (it->second.instances.Has(GetSlot(entity))) {
            auto& entry = it->second.instances[GetSlot(entity)];
            entry.reset();
        }
    }
}

Entity Registry::New()
{
    // First get a slot for the entity to use. If there are any slots in the pool
    // from dead entities, make use of that. Otherwise, get the next available.
    u16 slot = 0;
    if (!d_pool.empty()) {
        slot = d_pool.front();
        d_pool.pop();
    } else {
        slot = d_next++;
    }

    // Bump the version of this slot.
    u16 version = ++d_version[slot];

    u32 entity = GetID(slot, version);
    d_entities[slot] = entity;
    return {this, entity};
}

void Registry::Delete(Entity entity)
{
    assert(entity.Valid());

    // Clean up all components
    for (auto& [type, data] : d_comps) {
        Remove(entity.Id(), type);
    }

    d_entities[entity.Slot()] = NULL_ID;

    // Add the entity slot to the pool of available IDs.
    d_pool.push(entity.Slot());
}

void Registry::Clear()
{
    for (auto entity : All()) {
        entity.Delete();
    }
}

bool Registry::Valid(u32 entity) const
{
    u16 slot = GetSlot(entity);
    return entity != NULL_ID && d_entities[slot] == entity;
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

cppcoro::generator<Entity> Registry::All()
{
    for (auto entity : d_entities) {
        if (entity != NULL_ID) {
            co_yield {this, entity};
        }
    }
}

}
}