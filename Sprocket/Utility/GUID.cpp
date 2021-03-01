#include "GUID.h"

#include <fmt/core.h>

namespace Sprocket {
namespace guid {
namespace {

std::array<std::mt19937, 4> InitialiseEngines()
{
    std::random_device rd;
    std::array<std::mt19937, 4> engines{
        std::mt19937(rd()),
        std::mt19937(rd()),
        std::mt19937(rd()),
        std::mt19937(rd())
    };
    return engines;
}

}

Generator::Generator()
    : d_engines(InitialiseEngines())
    , d_distribution(1)
{}

GUID Generator::New()
{
    GUID guid{
        d_engines[0](),
        d_engines[1](),
        d_engines[2](),
        d_engines[3]()
    };
    return guid;
}

std::string Stringify(const GUID& guid)
{
    return fmt::format("{:x}-{:x}-{:x}-{:x}", guid[0], guid[1], guid[2], guid[3]);
}

}
}