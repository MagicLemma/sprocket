#include "Accessor.h"
#include "SceneManager.h"
#include "Window.h"

namespace Sprocket {

Accessor::Accessor(SceneManager* sm, Window* window)
    : d_sceneManager(sm)
    , d_window(window)
{
}

Accessor::Accessor(const Accessor& other)
    : d_sceneManager(other.d_sceneManager)
    , d_window(other.d_window)
{
}

Scene* Accessor::addScene(const std::string& name)
{
    return d_sceneManager->addScene(name);
}

bool Accessor::setActiveScene(const std::string& name)
{
    return d_sceneManager->setActiveScene(name);
}

bool Accessor::doesSceneExist(const std::string& name) const
{
    return d_sceneManager->doesSceneExist(name);
}

}