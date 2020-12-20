#include "ECS.h"
#include "Components.h"

#include <algorithm>

namespace Sprocket {
namespace ECS {

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
     return *this != ECS::Null
         && d_registry
         && d_registry->d_entities.Has(d_index)
         && d_registry->d_entities[d_index] == d_version;
}

void Entity::Delete() 
{
    if (Valid()) {
        // Clean up all components
        for (auto& [type, data] : d_registry->d_comps) {
            Remove(type);
        }
        d_registry->d_entities.Erase(Slot());

        // Add the entity slot to the pool of available IDs.
        d_registry->d_pool.push({d_index, d_version});
    }
}

u32 Entity::Id() const
{
    return (u32)d_version << 16 | d_index;
}

u16 Entity::Slot() const
{
    return d_index;
}

u16 Entity::Version() const
{
    return d_version;
}

Entity Entity::NewEntity() const
{
    return d_registry->New();
}

void Entity::Remove(std::type_index type)
{
    assert(Valid());
    for (const auto& cb : d_registry->d_comps[type].onRemove) {
        cb(*this);
    }

    if (auto it = d_registry->d_comps.find(type); it != d_registry->d_comps.end()) {
        if (it->second.instances.Has(d_index)) {
            it->second.instances.Erase(d_index);
        }
    }
}

Entity Registry::New()
{
    u16 index = 0;
    u16 version = 0;

    // If there is a slot in the pool, pop it and bump its version.
    if (!d_pool.empty()) {
        std::tie(index, version) = d_pool.front();
        d_pool.pop();
        ++version;
    }
    // Otherwise we append on the end.
    else {
        index = d_entities.Size(); // size of sparse == size of packed here
    }

    d_entities.Insert(index, version);
    return {this, index, version};
}

void Registry::Clear()
{
    // Clean up components, triggering onRemove behaviour
    for (auto entity : Safe()) {
        entity.Delete();
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

std::size_t Registry::Size() const
{
    return d_entities.Size();
}

cppcoro::generator<Entity> Registry::Fast()
{
    for (const auto& [index, version] : d_entities.Fast()) {
        co_yield {this, index, version};
    }
}

cppcoro::generator<Entity> Registry::Safe()
{
    for (const auto& [index, version] : d_entities.Safe()) {
        co_yield {this, index, version};
    }
}

}
}