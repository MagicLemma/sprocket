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

void Registry::Remove(u32 entity, std::type_index type)
{
    for (const auto& cb : d_comps[type].onRemove) {
        cb({this, entity});
    }

    if (auto it = d_comps.find(type); it != d_comps.end()) {
        if (it->second.instances.Has(GetSlot(entity))) {
            it->second.instances.Erase(GetSlot(entity));
        }
    }
}

Entity Registry::New()
{
    u16 slot = 0;
    u16 version = 0;

    // If there is a slot in the pool, pop it and bump its version.
    if (!d_pool.empty()) {
        std::tie(slot, version) = d_pool.front();
        d_pool.pop();
        ++version;
    }
    // Otherwise we append on the end.
    else {
        slot = d_entities.Size(); // size of sparse == size of packed here
    }

    d_entities.Insert(slot, version);
    return {this, GetID(slot, version)};
}

void Registry::Delete(Entity entity)
{
    assert(entity.Valid());

    // Clean up all components
    for (auto& [type, data] : d_comps) {
        Remove(entity.Id(), type);
    }
    d_entities.Erase(entity.Slot());

    // Add the entity slot to the pool of available IDs.
    d_pool.push({entity.Slot(), entity.Version()});
}

void Registry::Delete(const std::vector<Entity>& entities)
{
    for (auto entity : entities) {
        Delete(entity);
    }
}

void Registry::Clear()
{
    // Clean up components, triggering onRemove behaviour
    for (auto entity : Safe()) {
        Delete(entity);
    }

    // Reset internal structures
    d_entities.Clear();
    std::queue<std::pair<u16, u16>>().swap(d_pool);
    d_comps.clear();
}

Entity Registry::Find(const EntityPredicate& pred)
{
    for (auto entity : Fast()) {
        if (pred(entity)) {
            return entity;
        }
    }
    return ECS::Null;
}

bool Registry::Valid(u32 entity) const
{
    u16 index = GetSlot(entity);
    u16 version = GetVersion(entity);
    return entity != NULL_ID
        && d_entities.Has(index)
        && d_entities[index] == version;
}

std::size_t Registry::Size() const
{
    return d_entities.Size();
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

cppcoro::generator<Entity> Registry::Fast()
{
    for (const auto& [index, version] : d_entities.Fast()) {
        co_yield {this, GetID(index, version)};
    }
}

cppcoro::generator<Entity> Registry::Safe()
{
    for (const auto& [index, version] : d_entities.Safe()) {
        co_yield {this, GetID(index, version)};
    }
}

}
}