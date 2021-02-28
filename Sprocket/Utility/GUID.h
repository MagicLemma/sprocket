#pragma once
#include "Types.h"

#include <array>
#include <string>
#include <random>

namespace Sprocket {
namespace guid {

using GUID = std::array<u32, 4>;

class Generator
{
    std::array<std::mt19937, 4>        d_engines;
    std::uniform_int_distribution<u32> d_distribution;

public:
    Generator();

    GUID New();
};

// Returns a string representation of the GUID.
std::string Stringify(const GUID& guid);

// Returns a "zero" GUID, useful for default or null values.
static const GUID Zero{0, 0, 0, 0};

}
}

namespace std {

template <> struct hash<Sprocket::guid::GUID>
{
    std::size_t operator()(const Sprocket::guid::GUID& guid) const noexcept
    {
        std::hash<std::uint32_t> hasher;
        return hasher(guid[0]) ^ hasher(guid[1]) ^ hasher(guid[2]) ^ hasher(guid[3]);
    };
};

}