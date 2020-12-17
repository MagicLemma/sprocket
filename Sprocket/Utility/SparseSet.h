#pragma once
#include <cstdint>
#include <type_traits>
#include <vector>
#include <array>
#include <utility>

namespace Sprocket {

template <typename T, std::uint32_t Size>
class SparseSet
{
    static_assert(std::is_copy_constructible<T>());

    using packed = std::vector<std::pair<std::uint32_t, T>>;
    using sparse = std::array<std::uint32_t, Size>;

    packed d_elements;
    sparse d_slots;

public:
    SparseSet() 
    {
        for (auto& x : d_slots) {
            x = std::numeric_limits<std::uint32_t>::max();
        }
    }

    T& Insert(std::uint32_t slot, const T& value) {
        std::uint32_t location = d_elements.size();
        d_slots[slot] = location;
        return d_elements.emplace_back(std::make_pair(slot, value)).second;
    }

    bool Has(std::uint32_t slot) const {
        return d_slots[slot] != std::numeric_limits<std::uint32_t>::max();
    }

    // Removes the given slot. To keep the packed vector packaed, the back value
    // is moved into the slot of the outgoing v
    void Erase(std::uint32_t slot) {
        assert(Has(slot));

        if (d_slots[slot] == d_elements.size() - 1) {
            d_slots[slot] = std::numeric_limits<std::uint32_t>::max();
            d_elements.pop_back();
            return;
        }

        // Pop the back element of the sparse_list
        auto back = d_elements.back();
        d_elements.pop_back();

        // Get the index of the outgoing value within the elements vector.
        std::size_t index = d_slots[slot];
        d_slots[slot] = std::numeric_limits<std::uint32_t>::max();

        // Overwrite the outgoing value with the back value.
        d_elements[index] = back;

        // Point the slot for the back value to its new location.
        d_slots[back.first] = index;
    }

    std::size_t Size() const { return d_elements.size(); }

    T& operator[](std::uint32_t slot) {
        if (Has(slot)) {
            return d_elements[d_slots[slot]].second;
        }
        return Insert(slot, T{});
    }

    const T& operator[](std::uint32_t slot) const {
        assert(Has(slot));
        return d_elements[d_slots[slot]].second;
    }

    auto begin() { return d_elements.begin(); }
    auto end() { return d_elements.end(); }
};

}