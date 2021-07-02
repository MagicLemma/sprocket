#pragma once
#include "ECS.h"

#include <string>

namespace spkt {
namespace Loader {

void Save(const std::string& file, spkt::registry* reg);
void Load(const std::string& file, spkt::registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
apx::entity Copy(spkt::registry* scene, apx::entity entity);

// Copies one scene into another. The target scene is first cleared.
void Copy(spkt::registry* source, spkt::registry* target);

}
}