#include "ECS.h"
#include "Components.h"
#include "Log.h"

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

Registry::Registry()
    : d_next(0)
{}

void Registry::Remove(u32 entity, std::type_index type)
{
    for (const auto& cb : d_onRemoveCallbacks[type]) {
        cb({this, entity});
    }

    Handle handle = entity;
    if (auto it = d_components.find(type); it != d_components.end()) {
        auto& entry = it->second.at(handle.index);
        entry.reset();
    }
}

Entity Registry::New()
{
    Handle handle;
    if (auto it = d_pool.begin(); it != d_pool.end()) {
        handle.index = *it;
        d_pool.erase(it);
    }
    else {
        handle.index = d_next++;
        d_entities.push_back(handle.index);
    }
    handle.version = ++d_version[handle.index];
    return {this, handle.entity};
}

void Registry::Delete(Entity entity)
{
    Handle handle = entity.id;

    // Clean up all components
    for (auto& [type, components] : d_components) {
        Remove(entity.id, type);
    }

    // Remove the entity index from the "alive" list
    auto it = std::find(d_entities.begin(), d_entities.end(), handle.index);
    d_entities.erase(it);

    // Add the entity index to the pool of available IDs.
    d_pool.insert(handle.index);
}

bool Registry::Valid(u32 entity) const
{
    Handle handle = entity;
    return entity != ECS::Null.id
        && handle.index < d_next
        && !d_pool.contains(handle.index)
        && handle.version == d_version[handle.index];
}

void Registry::Sort(const Comparitor& compare)
{
    const auto c = [&](u16 a, u16 b) {
        Handle ha(a, d_version[a]);
        Handle hb(b, d_version[b]);
        return compare(ha.entity, hb.entity);
    };
    std::sort(d_entities.begin(), d_entities.end(), c);
}

std::size_t Registry::Size() const
{
    return d_next - d_pool.size();
}

Registry::Iterator& Registry::Iterator::operator++()
{
    ++d_dequeIndex;
    return *this;
}

Entity Registry::Iterator::operator*()
{
    Registry::Handle h;
    h.index = d_reg->d_entities[d_dequeIndex];
    h.version = d_reg->d_version[h.index];
    return {d_reg, h.entity};
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