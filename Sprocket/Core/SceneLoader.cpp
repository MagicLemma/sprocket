#include "SceneLoader.h"

#include <chrono>

namespace Sprocket {

SceneLoader::SceneLoader()
    : d_scene(nullptr)
{
}

void SceneLoader::loadScene(SceneLoaderJob job)
{
    d_runningJob = std::async(std::launch::async, [job]() -> std::unique_ptr<Scene> {
        return job();
    });
}

bool SceneLoader::isSceneAvailable()
{
    return d_runningJob.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

std::unique_ptr<Scene> SceneLoader::getScene()
{
    std::unique_ptr<Scene> newScene = std::move(d_scene);
    d_scene = nullptr;
    return newScene;
}

}