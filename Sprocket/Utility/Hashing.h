#pragma once
#include <utility>
#include <string_view>

namespace spkt {

struct hash_pair { 
    template <class T1, class T2> 
    std::size_t operator()(const std::pair<T1, T2>& p) const
    { 
        auto hash1 = std::hash<T1>{}(p.first); 
        auto hash2 = std::hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
};

template <typename T>
constexpr std::size_t sdbm_type_hash()
{
    std::size_t hash = 0;
    for (const auto& c : std::string_view{__FUNCSIG__}) {
        hash = static_cast<std::size_t>(c) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
};

template <typename T>
constexpr std::size_t type_hash = sdbm_type_hash<T>();

static_assert(type_hash<int> != 0); // Check that it is actually calcualated at compile time

}