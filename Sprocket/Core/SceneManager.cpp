#include "SceneManager.h"
#include "Log.h"
#include "Scene.h"

#include <memory>

namespace Sprocket {

SceneManager::SceneManager()
    : d_scene()
{

}


void SceneManager::OnUpdate(double dt)
{
    d_scene.OnUpdate(dt);
}

void SceneManager::OnEvent(Event& event)
{
    d_scene.OnEvent(event);
}

}