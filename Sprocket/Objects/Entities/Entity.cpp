#include "Entity.h"

namespace Sprocket {

namespace {

std::size_t idGenerator()
{
    static std::size_t id = 0;
    return id++;
}

}

Entity::Entity()
    : d_id(idGenerator())
    , d_alive(true)
    , d_position({0, 0, 0})
    , d_orientation(Maths::identity)
{
    for (std::size_t i = 0; i != MAX_COMPONENTS; ++i) {
        d_components[i] = nullptr;
    }
}

Maths::mat4 Entity::transform() const
{
    return Maths::transform(d_position, d_orientation);
}

}