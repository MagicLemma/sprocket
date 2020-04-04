#pragma once
#include "Model3D.h"

#include <string>

namespace Sprocket {

class Scene;
class SceneManager;
class Window;

class Accessor
// A wrapper class for several key systems in Sprocket to expose
// a limited subset of functionality to Scenes and Layers.
{
    SceneManager*    d_sceneManager;
    Window*          d_window;

public:
    Accessor(SceneManager* sm, Window* window);
    Accessor(const Accessor& other); // Copy-constructor

    // SceneManager functionality
    Scene* addScene(const std::string& name);
    bool setActiveScene(const std::string& name);
    bool doesSceneExist(const std::string& name) const;

    // Window functionality
    Window* window() const { return d_window; }

};

}