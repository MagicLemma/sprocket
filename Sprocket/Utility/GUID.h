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

std::string Stringify(const GUID& guid);

GUID Zero();

}
}