#pragma once
#include <Sprocket/Scene/ecs.h>

#include <string>
#include <functional>

namespace spkt {

using save_predicate = std::function<bool(const spkt::registry&, spkt::entity)>;

// Saves all entities satisfying the given predicate to the given file.
void save_registry_to_file(
    const std::string& file,
    const spkt::registry& reg,
    const save_predicate& predicate
);

void load_registry_from_file(const std::string& file, spkt::registry& reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
spkt::entity copy_entity(spkt::registry& scene, spkt::entity entity);

// Copies one scene into another. The target scene is first cleared.
void copy_registry(const spkt::registry& source, spkt::registry& target);

}