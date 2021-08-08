#pragma once
#include <cstddef>
#include <ranges>

namespace spkt {
namespace views {

constexpr auto enumerate(std::size_t i = 0)
{
    return std::views::transform([i](const auto& element) mutable {
        return std::make_pair(i++, std::cref(element));
    });
}

}
}