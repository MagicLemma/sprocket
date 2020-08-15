#pragma once
#include "EntityManager.h"

#include <string>

namespace Sprocket {

class Serialiser
{
    EntityManager* d_scene;

public:
    Serialiser(EntityManager* scene) : d_scene(scene) {}

    void Serialise(const std::string& file);
        // Writes the current scene to the given file.

    void Deserialise(const std::string& file);
        // Loads the scene described in the given file into the scene
        // object. The scene object is cleared first.
};
    
}