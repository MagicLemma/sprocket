#include "Accessor.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Window.h"

namespace Sprocket {

Accessor::Accessor(SceneManager* sm, ResourceManager* rm, Window* window)
    : d_sceneManager(sm)
    , d_resourceManager(rm)
    , d_window(window)
{
}

Accessor::Accessor(const Accessor& other)
    : d_sceneManager(other.d_sceneManager)
    , d_resourceManager(other.d_resourceManager)
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

Model3D Accessor::loadModel3D(const std::string& name,
                                          const std::string& objFile)
{
    return d_resourceManager->loadModel3D(name, objFile);
}

Model3D Accessor::getModel3D(const std::string& name) const
{
    return d_resourceManager->getModel3D(name);
}

}