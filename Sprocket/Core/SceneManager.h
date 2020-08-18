#pragma once
#include "Scene.h"
#include "Event.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace Sprocket {

using SceneManagerJob = std::function<void(SceneManager*)>;

class SceneManager
{
    Scene d_scene;

    SceneManager(SceneManager&&) = delete;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
        // SceneManagers are non-copyable and non-moveable.

public:
    SceneManager();

    Scene* Scene() { return &d_scene; };
        // Create a new scene with the given name and return a pointer to it.

    void OnUpdate(double dt);
        // Updates and draws the active scene.

    void OnEvent(Event& event);
        // Sends the event to the active scene.
};

}