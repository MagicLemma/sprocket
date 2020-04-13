#include "Entity.h"

namespace Sprocket {

Entity::Entity()
{
    for (std::size_t i = 0; i != MAX_COMPONENTS; ++i) {
        d_components[i] = nullptr;
    }
}

}