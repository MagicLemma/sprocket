#include "Layer.h"

#include <chrono>

namespace Sprocket {

Layer::Layer(bool initialActivation)
    : d_active(initialActivation)
{
}

bool Layer::isActive() const
{
    return d_active;
}

}