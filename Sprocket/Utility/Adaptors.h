#pragma once
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

}