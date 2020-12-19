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
            auto& entry = it->second.instances[GetSlot(entity)];
            entry.reset();
        }
    }
}

Entity Registry::New()
{
    u16 slot = 0;
    u16 version = 0;

    // If there is a slot in the pool, pop it and bump its version.
    if (auto it = d_pool.begin(); it != d_pool.end()) {
        slot = *it;
        d_pool.erase(it);
        version = ++d_entities[slot];
    }
    // Otherwise we append on the end.
    else {
        slot = d_entities.size();
        d_entities.push_back(version);
    }

    return {this, GetID(slot, version)};
}

void Registry::Delete(Entity entity)
{
    assert(entity.Valid());

    // Clean up all components
    for (auto& [type, data] : d_comps) {
        Remove(entity.Id(), type);
    }

    // Add the entity slot to the pool of available IDs.
    d_pool.insert(entity.Slot());
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
    u16 version = GetVersion(entity);
    return entity != NULL_ID
        && d_entities[slot] == version
        && !d_pool.contains(slot);
}

std::size_t Registry::Size() const
{
    return d_entities.size() - d_pool.size();
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
    for (u16 index = 0; index < d_entities.size(); ++index) {
        if (!d_pool.contains(index)) {
            co_yield {this, GetID(index, d_entities[index])};
        }
    }
}

}
}