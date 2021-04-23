#pragma once
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <vector>
#include <array>
#include <utility>
#include <functional>

#include <apecs.hpp>

namespace Sprocket {

template <typename ValueType, typename IndexType = std::uint32_t>
class SparseSet
{
public:
    using PackedType = std::vector<std::pair<IndexType, ValueType>>;
    using SparseType = std::vector<IndexType>;

    using Iterator = typename PackedType::iterator;

private:
    static_assert(std::is_default_constructible<ValueType>());
    static_assert(std::is_copy_constructible<ValueType>());
    static_assert(std::is_integral<IndexType>());

    static const IndexType EMPTY = std::numeric_limits<IndexType>::max();

    PackedType d_packed;
    SparseType d_sparse;

    // Grows the sparse vector so that the given index becomes valid.
    void Assure(IndexType index);

public:
    SparseSet() = default;

    // Inserts the given value into the specified index. If a value already
    // exists at that index, it is overwritten.
    ValueType& Insert(IndexType index, const ValueType& value);

    // Returns true if the specified index contains a value, and false otherwise.
    bool Has(IndexType index) const;

    // Removes all elements from the set.
    void Clear();

    // Removes the value at the specified index. The structure may reorder
    // itself to maintain contiguity for iteration.
    void Erase(IndexType index);

    std::size_t Size() const;

    ValueType& operator[](IndexType index);
    const ValueType& operator[](IndexType index) const;

    // Provides a generator that loops over the packed set, which is fast but
    // results in undefined behaviour when removing elements.
    apx::generator<std::pair<const IndexType, ValueType>&> Fast();

    // Provides a generator that loops over the sparse set, which is slower but
    // safe for deleting the current element.
    apx::generator<std::pair<const IndexType, ValueType>&> Safe();
};


template <typename ValueType, typename IndexType>
void SparseSet<ValueType, IndexType>::Assure(IndexType index) {
    if (d_sparse.size() <= index) {
        d_sparse.resize(index + 1, EMPTY);
    }
}

template <typename ValueType, typename IndexType>
ValueType& SparseSet<ValueType, IndexType>::Insert(IndexType index, const ValueType& value)
{
    assert(!Has(index));
    Assure(index);
    IndexType location = d_packed.size();
    d_sparse[index] = location;
    return d_packed.emplace_back(std::make_pair(index, value)).second;
}

template <typename ValueType, typename IndexType>
bool SparseSet<ValueType, IndexType>::Has(IndexType index) const
{
    return index < d_sparse.size() && d_sparse[index] != EMPTY;
}

template <typename ValueType, typename IndexType>
void SparseSet<ValueType, IndexType>::Clear()
{
    d_packed.clear();
    d_sparse.clear();
}

template <typename ValueType, typename IndexType>
void SparseSet<ValueType, IndexType>::Erase(IndexType index)
{
    assert(Has(index));

    if (d_sparse[index] == d_packed.size() - 1) {
        d_sparse[index] = EMPTY;
        d_packed.pop_back();
        return;
    }

    // Pop the back element of the sparse_list
    auto back = d_packed.back();
    d_packed.pop_back();

    // Get the index of the outgoing value within the elements vector.
    std::size_t packed_index = d_sparse[index];
    d_sparse[index] = EMPTY;

    // Overwrite the outgoing value with the back value.
    d_packed[packed_index] = back;

    // Point the index for the back value to its new location.
    d_sparse[back.first] = packed_index;
}

template <typename ValueType, typename IndexType>
std::size_t SparseSet<ValueType, IndexType>::Size() const
{
    return d_packed.size();
}

template <typename ValueType, typename IndexType>
ValueType& SparseSet<ValueType, IndexType>::operator[](IndexType index)
{
    if (Has(index)) {
        return d_packed[d_sparse[index]].second;
    }
    return Insert(index, ValueType{});
}

template <typename ValueType, typename IndexType>
const ValueType& SparseSet<ValueType, IndexType>::operator[](IndexType index) const
{
    assert(Has(index));
    return d_packed[d_sparse[index]].second;
}

template <typename ValueType, typename IndexType>
apx::generator<std::pair<const IndexType, ValueType>&> SparseSet<ValueType, IndexType>::Fast()
{
    for (auto pair : d_packed) {
        co_yield pair;
    }
}

template <typename ValueType, typename IndexType>
apx::generator<std::pair<const IndexType, ValueType>&> SparseSet<ValueType, IndexType>::Safe()
{
    for (auto index : d_sparse) {
        if (index != EMPTY) {
            co_yield d_packed[index];
        }
    }
}

}