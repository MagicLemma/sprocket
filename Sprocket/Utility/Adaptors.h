#pragma once
#include <optional>
#include <functional>
#include <vector>
#include <utility>

#include <cppcoro/generator.hpp>

namespace itertools {

template <typename T>
cppcoro::generator<typename T::reference>
reversed(T& iterable)
{
    for (auto it = iterable.rbegin(); it != iterable.rend(); ++it) {
        co_yield *it;
    }
}

template <typename T>
cppcoro::generator<typename T::const_reference>
reversed(const T& iterable)
{
    for (auto it = iterable.crbegin(); it != iterable.crend(); ++it) {
        co_yield *it;
    }
}

template <typename T>
cppcoro::generator<std::pair<const std::size_t, typename T::reference>>
enumerate(T& iterable, std::size_t start = 0)
{
    std::size_t index = start;
    for (auto& elem : iterable) {
        co_yield {index, elem};
        ++index;
    }
}

template <typename T>
cppcoro::generator<std::pair<const std::size_t, typename T::const_reference>>
enumerate(const T& iterable, std::size_t start = 0)
{
    std::size_t index = start;
    for (const auto& elem : iterable) {
        co_yield {index, elem};
        ++index;
    }
}

template <typename T>
cppcoro::generator<T>
count(const T& start = T{0}, const T& step = T{1})
{
    T val = start;
    while (true) {
        co_yield val;
        val += step;
    }
}

template <typename T>
cppcoro::generator<typename T::const_reference>
cycle(const T& iterable)
{
    while (true) {
        for (const auto& elem : iterable) {
            co_yield elem;
        }
    }
}

template <typename T>
cppcoro::generator<const T&>
repeat(const T& object, const std::optional<std::size_t>& times = {})
{
    if (times.has_value()) {
        for (std::size_t i = 0; i != times; ++i) {
            co_yield object;
        }
    } else {
        while (true) {
            co_yield object;
        }
    }
}

template <typename T>
cppcoro::generator<typename T::const_reference>
filterfalse(std::function<bool(typename T::const_reference)> predicate, const T& iterable)
{
    for (const auto& elem : iterable) {
        if (!predicate(elem)) {
            co_yield elem;
        }
    }
}

template <typename T>
cppcoro::generator<typename T::const_reference>
filterfalse(const T& iterable)
{
    for (const auto& elem : iterable) {
        if (!elem) {
            co_yield elem;
        }
    }
}

}