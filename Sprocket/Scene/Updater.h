#pragma once

#include <yaml-cpp/yaml.h>

namespace Sprocket {

// Given a loaded scene, check the version. If the version is out of
// date, this function will apply conversions to being it up to date.
void UpdateScene(YAML::Node* scene);

}