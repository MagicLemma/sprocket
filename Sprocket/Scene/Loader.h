#pragma once
#include "ECS.h"
#include "Scene.h"

#include <memory>
#include <string>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, std::shared_ptr<Scene> scene);
void Load(const std::string& file, std::shared_ptr<Scene> scene);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
Entity Copy(std::shared_ptr<Scene> scene, Entity entity);

// Copies one scene into another. The target scene is first cleared.
void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target);

}
}