#pragma once
#include <cstddef>
#include <memory>
#include <string_view>

namespace spkt {

template <typename T>
constexpr std::string_view type_name_raw() {
    return __FUNCSIG__;
}

template <typename T>
constexpr std::string_view type_name() {
    // Calculate how many chars are needed to be stripped from the front and
    // back of the raw string by counting using a known type.
    constexpr std::string_view float_raw = type_name_raw<float>();
    constexpr std::string_view float_parsed = "float";

    constexpr std::size_t num_prefix_chars = float_raw.find(float_parsed);
    static_assert(num_prefix_chars >= 0);

    constexpr std::size_t num_suffix_chars = float_raw.size() - num_prefix_chars - float_parsed.size();
    static_assert(num_suffix_chars >= 0);

    std::string_view raw = type_name_raw<T>();
    raw.remove_prefix(num_prefix_chars);
    raw.remove_suffix(num_suffix_chars);
    return raw;
}

template <typename T>
constexpr std::size_t sdbm_type_hash()
{
    std::size_t hash = 0;
    for (const auto& c : type_name_raw<T>()) {
        hash = c + 65599 * hash;
    }
    return hash;
};

struct type_info_t
{
    const std::string_view name;
    const std::size_t      hash;

    template <typename T>
    explicit type_info_t(std::in_place_type_t<T>)
        : name(type_name<T>())
        , hash(sdbm_type_hash<T>())
    {}

    bool operator==(const type_info_t& other) const
    {
        return name == other.name && hash == other.hash;
    }
};

template <typename T>
inline const type_info_t type_info = type_info_t(std::in_place_type<T>);

}

template <>
struct std::hash<spkt::type_info_t>
{
    std::size_t operator()(const spkt::type_info_t& info) const noexcept
    {
        return info.hash;
    };
};