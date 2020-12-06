#pragma once
#include "Types.h"

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <array>
#include <memory>
#include <unordered_set>
#include <functional>
#include <deque>
#include <limits>

namespace Sprocket {
namespace ECS {

constexpr u64 Null = std::numeric_limits<u64>::max();

class Registry
{
public:
    using Opaque = std::unique_ptr<void, std::function<void(void*)>>;
    using Comparitor = std::function<bool(u64, u64)>;

private:

    union Handle
    {
        struct {
            u32 index;
            u32 version;
        };
        u64 entity;

        Handle(u64 e) : entity(e) {}
        Handle(u32 i, u32 v) : index(i), version(v) {}
        Handle() : entity(0) {}
    };

    static constexpr std::size_t NUM_ENTITIES = 100000;

    // When an entity is removed, their ID is added to the pool so that it can be reused.
    std::unordered_set<u32> d_pool;

    // If the pool of IDs is empty, then the next entity will use this variable as their ID.
    u32 d_next;

    // We also keep track of the number of times an entity ID has been used. This is used to
    // check if a Entity handle is still valid. If the entity
    std::array<u32, NUM_ENTITIES> d_version;

    // This deque contains the currently alive entities. This is used for iteration.
    std::deque<u32> d_entities;

    // All the components are stored in contiguous lists. This indexes into the arrays are
    // the u32 indices that identify a component.
    std::unordered_map<std::type_index, std::array<Opaque, NUM_ENTITIES>> d_components;

public:
    Registry();

    u64 New();
    void Delete(u64 entity);

    bool Valid(u64 entity) const;

    void Sort(const Comparitor& compare);

    template <typename Comp>
    void Sort(const Comparitor& compare);

    u32 Size() const;

    template <typename Comp> Comp& Add(u64 entity, const Comp& component);
    template <typename Comp> void Remove(u64 entity);
    template <typename Comp> Comp& Get(u64 entity);
    template <typename Comp> const Comp& Get(u64 entity) const;
    template <typename Comp> bool Has(u64 entity) const;

    class Iterator
    {
        Registry*   d_reg;
        std::size_t d_dequeIndex;

    public:
        Iterator(Registry* reg, std::size_t index) : d_reg(reg), d_dequeIndex(index) {}
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        u64 operator*();
    };

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, d_entities.size()); }

    template <typename Comp>
    class ViewType
    {
        Registry* d_reg;

    public:
        class ViewIterator
        {
            Registry* d_reg;
            Iterator  d_iter;

        public:
            ViewIterator(Registry* reg, const Iterator& iter) : d_reg(reg), d_iter(iter) {
                while (d_iter != d_reg->end() && !d_reg->Has<Comp>(*d_iter)) ++d_iter;
            }
            ViewIterator& operator++();
            bool operator==(const ViewIterator& other) const;
            bool operator!=(const ViewIterator& other) const;
            u64 operator*();
        };

        ViewType(Registry* reg) : d_reg(reg) {}

        ViewIterator begin() { return ViewIterator(d_reg, d_reg->begin()); }
        ViewIterator end() { return ViewIterator(d_reg, d_reg->end()); }
    };

    template <typename Comp>
    ViewType<Comp> View()
    {
        return ViewType<Comp>(this);
    }

    friend class Iterator;
};

// ==============================================================
//                      TEMPLATE DEFINITIONS
// ==============================================================

template <typename Comp>
void Registry::Sort(const Comparitor& compare)
{
    Sort([&](u64 a, u64 b) {
        bool ac = Has<Comp>(a);
        bool bc = Has<Comp>(b);
        if (ac && bc) {
            return compare(a, b);
        }
        return ac; // Comp will shift to the front.
    });
}

template <typename Comp>
Comp& Registry::Add(u64 entity, const Comp& component)
{
    static constexpr auto Deleter = [](void* data) {
        if (data) delete static_cast<Comp*>(data);
    };

    Handle handle = entity;
    auto& entry = d_components[typeid(Comp)][handle.index];
    entry = Opaque(new Comp(component), Deleter);
    return *static_cast<Comp*>(entry.get());
}

template <typename Comp>
void Registry::Remove(u64 entity)
{
    Handle handle = entity;
    if (auto it = d_components.find(typeid(Comp)); it != d_components.end()) {
        auto& entry = it->second.at(handle.index);
        entry.reset();
    }
}

template <typename Comp>
Comp& Registry::Get(u64 entity)
{
    Handle handle = entity;
    const auto& entry = d_components.at(typeid(Comp)).at(handle.index);
    return *static_cast<Comp*>(entry.get());
}

template <typename Comp>
const Comp& Registry::Get(u64 entity) const
{
    Handle handle = entity;
    const auto& entry = d_components.at(typeid(Comp)).at(handle.index);
    return *static_cast<Comp*>(entry.get());
}

template <typename Comp>
bool Registry::Has(u64 entity) const
{
    Handle handle = entity;
    if (auto it = d_components.find(typeid(Comp)); it != d_components.end()) {
        const auto& entry = it->second.at(handle.index);
        return entry != nullptr;
    }
    return false;
}

template <typename Comp>
typename Registry::ViewType<Comp>::ViewIterator& Registry::ViewType<Comp>::ViewIterator::operator++()
{
    ++d_iter;
    while (d_iter != d_reg->end() && !d_reg->Has<Comp>(*d_iter)) ++d_iter;
    return *this;
}

template <typename Comp>
bool Registry::ViewType<Comp>::ViewIterator::operator==(const Registry::ViewType<Comp>::ViewIterator& other) const
{
    return d_iter == other.d_iter;
}

template <typename Comp>
bool Registry::ViewType<Comp>::ViewIterator::operator!=(const Registry::ViewType<Comp>::ViewIterator& other) const
{
    return !(*this == other);
}

template <typename Comp>
u64 Registry::ViewType<Comp>::ViewIterator::operator*()
{
    return *d_iter;
}

}
}