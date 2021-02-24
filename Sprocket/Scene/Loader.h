#pragma once
#include "ECS.h"

#include <string>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, ecs::Registry* reg);
void Load(const std::string& file, ecs::Registry* reg);

// Creates a duplicate of the given entity in the given scene.
// The scene does not need to be the scene that contains the given
// entity. The ID of the new entity may be different.
ecs::Entity Duplicate(ecs::Registry* reg, ecs::Entity entity);

// Copies one scene into another. The target scene is first cleared.
// Entity IDs are preserved.
void Copy(ecs::Registry* source, ecs::Registry* target);

}
}