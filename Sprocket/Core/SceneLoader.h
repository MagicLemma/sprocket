#pragma once
#include "Scene.h"

#include <memory>
#include <functional>
#include <future>

namespace Sprocket {

using SceneLoaderJob = std::function<std::unique_ptr<Scene>()>;

class SceneLoader
{
    std::unique_ptr<Scene> d_scene;

    std::future<std::unique_ptr<Scene>> d_runningJob;

public:
    SceneLoader();

    void loadScene(SceneLoaderJob job);
    bool isSceneAvailable();
    std::unique_ptr<Scene> getScene();
};


}