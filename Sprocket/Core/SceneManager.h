#pragma once
#include "Scene.h"
#include "Event.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

namespace Sprocket {

using SceneManagerJob = std::function<void(SceneManager*)>;

class SceneManager
{
    mutable std::mutex d_jobMutex;
    std::vector<SceneManagerJob> d_jobs;

    mutable std::mutex d_sceneMutex;
    std::unordered_map<std::string, std::unique_ptr<Scene>> d_scenes;

    mutable std::mutex d_activeSceneMutex;
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

    void handleEvent(const Event& event);
        // Sends the event to the active scene.

    void draw();
        // Draws the active scene.

    void addJob(SceneManagerJob job);
    void flushJobs();
};

}