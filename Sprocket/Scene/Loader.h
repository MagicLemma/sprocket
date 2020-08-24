#pragma once
#include "Entity.h"

#include <memory>
#include <string>

namespace Sprocket {
namespace Loader {

void Save(const std::string& file, std::shared_ptr<Scene> scene);
void Load(const std::string& file, std::shared_ptr<Scene> scene);

void Copy(std::shared_ptr<Scene> source, std::shared_ptr<Scene> target);

}
}