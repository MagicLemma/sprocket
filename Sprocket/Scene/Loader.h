#pragma once
#include "apecs.hpp"

#include <string>

namespace spkt {

void save_registry_to_file(const std::string& file, apx::registry* reg);
void load_registry_from_file(const std::string& file, apx::registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
apx::entity copy_entity(apx::registry* scene, apx::entity entity);

// Copies one scene into another. The target scene is first cleared.
void copy_registry(apx::registry* source, apx::registry* target);

}