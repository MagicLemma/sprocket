#pragma once
#include "Types.h"

#include <vector>

namespace Sprocket {

template <typename Iterable>
class Reversed
{
    Iterable& d_container;

public:
    Reversed(Iterable& container) : d_container(container) {}

    auto begin() { return d_container.rbegin(); }
    auto end() { return d_container.rend(); }

    auto cbegin() { return d_container.crbegin(); }
    auto cend() { return d_container.crend(); }
};

template <typename Iterable>
class Enumerate
{
    Iterable& d_container;
    u32 d_counter;

public:
    class Iterator
    {
        using ValueType = typename Iterable::value_type;
        using InnerIterator = typename Iterable::iterator;
        InnerIterator d_iterator;
        u32 d_value;

    public:
        Iterator(const InnerIterator& iterator, u32 value) : d_iterator(iterator), d_value(value) {}
        auto operator*() { return std::make_pair(d_value, std::ref(*d_iterator)); }
        
        Iterator& operator++() {
            ++d_iterator;
            ++d_value;
            return *this;
        }

        bool operator==(const Iterator& other) const { return d_iterator == other.d_iterator && d_value == other.d_value; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    Enumerate(Iterable& container, u32 start = 0) : d_container(container), d_counter(start) {}

    Iterator begin() { return Iterator(d_container.begin(), 0); }
    Iterator end() { return Iterator(d_container.end(), d_container.size()); }
};

}