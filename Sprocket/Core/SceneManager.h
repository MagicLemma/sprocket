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
    std::unordered_map<std::string, std::unique_ptr<Scene>> d_scenes;

    std::string d_activeSceneName;

    SceneManager(SceneManager&&) = delete;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
        // SceneManagers are non-copyable and non-moveable.

public:
    SceneManager();

    Scene* addScene(const std::string& name);
        // Create a new scene with the given name and return a pointer to it.

    bool setActiveScene(const std::string& name);
        // Set the scene with the given name to be the active scene. If no
        // scene with the given name exists, this is a no-op and false is
        // returned.

    bool doesSceneExist(const std::string& name) const;

    void update();
        // Updates the active scene.

    void handleEvent(Event& event);
        // Sends the event to the active scene.

    void draw();
        // Draws the active scene.
};

}