#pragma once
#include "ECS.h"

#include <string>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, ECS::Registry* reg);
void Load(const std::string& file, ECS::Registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
ECS::Entity Copy(ECS::Registry* scene, ECS::Entity entity);

// Copies one scene into another. The target scene is first cleared.
void Copy(ECS::Registry* source, ECS::Registry* target);

}
}