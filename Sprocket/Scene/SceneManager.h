#pragma once
#include "Scene.h"
#include "Event.h"

#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <entt/entt.hpp>

namespace Sprocket {

class SceneManager
{
    std::shared_ptr<Scene> d_activeScene;

public:
    SceneManager();

    void OnUpdate(double dt);
    void OnEvent(Event& event);

    void Start();
    void Pause();
    void Unpause();

    void ClearScene();

    void Set(std::shared_ptr<Scene> scene);
};

}