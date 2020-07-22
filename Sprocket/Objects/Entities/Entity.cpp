#include "Entity.h"

namespace Sprocket {

namespace {

std::size_t IdGenerator()
{
    static std::size_t id = 0;
    return id++;
}

}

Entity::Entity()
    : d_name("Unnamed")
    , d_id(IdGenerator())
    , d_alive(true)
{
    for (std::size_t i = 0; i != MAX_COMPONENTS; ++i) {
        d_components[i] = nullptr;
    }
}

}