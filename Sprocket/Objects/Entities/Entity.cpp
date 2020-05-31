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
    , d_position({0, 0, 0})
    , d_orientation(Maths::identity)
{
    for (std::size_t i = 0; i != MAX_COMPONENTS; ++i) {
        d_components[i] = nullptr;
    }
}

Maths::mat4 Entity::Transform() const
{
    return Maths::Transform(d_position, d_orientation);
}

}