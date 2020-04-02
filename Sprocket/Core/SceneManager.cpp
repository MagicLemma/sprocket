#include "SceneManager.h"
#include "Log.h"

namespace Sprocket {

SceneManager::SceneManager()
    : d_scenes()
    , d_activeScene(nullptr)
{

}

Scene* SceneManager::addScene(const std::string& name)
{
    auto scene = d_scenes.insert(std::make_pair(name, std::make_unique<Scene>()));

    if (scene.second == false) {
        SPKT_LOG_ERROR("A scene with that name already exists!");
        return nullptr;
    }

    return scene.first->second.get();
}

bool SceneManager::setActiveScene(const std::string& name)
{
    auto scene = d_scenes.find(name);
    if (scene == d_scenes.end()) {
        SPKT_LOG_FATAL("Could not set {} as the scene as it does not exist!");
        return false;
    }

    d_activeScene = scene->second.get();
}

bool SceneManager::doesSceneExist(const std::string& name) const
{
    return d_scenes.find(name) != d_scenes.end();
}

void SceneManager::update()
{
    if (!d_activeScene) {
        SPKT_LOG_FATAL("No active scene to update!");
        return;
    }

    d_activeScene->update();
}

void SceneManager::handleEvent(const Event& event)
{
    if (d_activeScene) {
        d_activeScene->handleEvent(event);
    }
}

void SceneManager::draw()
{
    if (!d_activeScene) {
        SPKT_LOG_FATAL("No active scene to draw!");
        return;
    }

    d_activeScene->draw();
}

}