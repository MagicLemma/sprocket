#pragma once
#include "ECS.h"

#include <string>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, ecs::Registry* reg);
void Load(const std::string& file, ecs::Registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
ecs::Entity Copy(ecs::Registry* scene, ecs::Entity entity);

// Copies one scene into another. The target scene is first cleared.
void Copy(ecs::Registry* source, ecs::Registry* target);

}
}