#pragma once
#include "Sprocket.h"

#include <vector>

struct BasicSceneInfo
{
    Sprocket::Window* window;
    Sprocket::FirstPersonCamera camera;
    Sprocket::PerspectiveLens   lens;

    Sprocket::Skybox skybox;
 
    std::vector<Sprocket::Entity>   entities;
    std::vector<Sprocket::Terrain>  terrains;
    std::vector<Sprocket::Light>    lights;

    std::vector<Sprocket::Model2D> models;
    
    bool paused = false;
    
    BasicSceneInfo(Sprocket::Window* window);
};