#pragma once
#include <cstdint>
#include <type_traits>
#include <vector>
#include <array>
#include <utility>

namespace Sprocket {

template <typename ValueType, typename IndexType = std::uint32_t>
class SparseSet
{
    static_assert(std::is_default_constructible<ValueType>());
    static_assert(std::is_copy_constructible<ValueType>());
    static_assert(std::is_integral<IndexType>());

    const IndexType EMPTY = std::numeric_limits<IndexType>::max();

    using packed = std::vector<std::pair<IndexType, ValueType>>;
    using sparse = std::vector<IndexType>;

    packed d_elements;
    sparse d_slots;

    void Assure(IndexType slot) {
        if (d_slots.size() <= slot) {
            d_slots.resize(slot + 1, EMPTY);
        }
    }

public:
    SparseSet() = default;

    ValueType& Insert(IndexType slot, const ValueType& value) {
        if (Has(slot)) {
            return d_elements[d_slots[slot]].second = value;
        }
        else {
            Assure(slot);
            IndexType location = d_elements.size();
            d_slots[slot] = location;
            return d_elements.emplace_back(std::make_pair(slot, value)).second;
        }
    }

    bool Has(IndexType slot) const {
        return slot < d_slots.size() && d_slots[slot] != EMPTY;
    }

    void Clear() {
        d_elements.clear();
        d_slots.clear();
    }

    // Removes the given slot. To keep the packed vector packaed, the back value
    // is moved into the slot of the outgoing v
    void Erase(IndexType slot) {
        assert(Has(slot));

        if (d_slots[slot] == d_elements.size() - 1) {
            d_slots[slot] = EMPTY;
            d_elements.pop_back();
            return;
        }

        // Pop the back element of the sparse_list
        auto back = d_elements.back();
        d_elements.pop_back();

        // Get the index of the outgoing value within the elements vector.
        std::size_t index = d_slots[slot];
        d_slots[slot] = EMPTY;

        // Overwrite the outgoing value with the back value.
        d_elements[index] = back;

        // Point the slot for the back value to its new location.
        d_slots[back.first] = index;
    }

    std::size_t Size() const { return d_elements.size(); }

    ValueType& operator[](IndexType slot) {
        if (Has(slot)) {
            return d_elements[d_slots[slot]].second;
        }
        return Insert(slot, ValueType{});
    }

    const ValueType& operator[](IndexType slot) const {
        assert(Has(slot));
        return d_elements[d_slots[slot]].second;
    }

    auto begin() { return d_elements.begin(); }
    auto end() { return d_elements.end(); }
};

}