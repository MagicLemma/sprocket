#pragma once
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "EntitySystem.h"

namespace Sprocket {
namespace ECS {

class SystemList
{
    std::unordered_map<std::type_index, EntitySystem> d_systems;
};

}
}