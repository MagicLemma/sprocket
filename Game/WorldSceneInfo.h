#pragma once
#include "Sprocket.h"

#include <vector>

struct BasicSceneInfo
{
    Sprocket::Window* window;
    Sprocket::FirstPersonCamera firstCamera;
    Sprocket::ThirdPersonCamera thirdCamera;
    Sprocket::Camera* camera;
    
    Sprocket::PerspectiveLens   lens;

    Sprocket::Skybox skybox;
 
    std::vector<Sprocket::Entity>   entities;
    std::vector<Sprocket::Terrain>  terrains;
    std::vector<Sprocket::Light>    lights;
    
    bool cameraIsFirst = true;
    bool paused = false;
    
    BasicSceneInfo(Sprocket::Window* window);
};