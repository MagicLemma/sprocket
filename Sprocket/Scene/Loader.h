#pragma once
#include "apecs.hpp"

#include <string>

namespace spkt {
namespace Loader {

void Save(const std::string& file, apx::registry* reg);
void Load(const std::string& file, apx::registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
apx::entity Copy(apx::registry* scene, apx::entity entity);

// Copies one scene into another. The target scene is first cleared.
void Copy(apx::registry* source, apx::registry* target);

}
}