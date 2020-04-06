#pragma once
#include <Sprocket.h>

#include <memory>
#include <random>

class WorldLayer : public Sprocket::Layer
{
    Sprocket::FirstPersonCamera d_firstCamera;
    Sprocket::ThirdPersonCamera d_thirdCamera;
    Sprocket::Camera* d_camera;
    bool d_cameraIsFirst = true;

    Sprocket::PerspectiveLens d_lens;

    Sprocket::Skybox d_skybox;
 
    Sprocket::EntityManager d_entityManager;
    
    std::vector<Sprocket::Terrain> d_terrains;
    std::vector<Sprocket::Light>   d_lights;
    
    bool d_paused = false;

    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;
    Sprocket::SkyboxRenderer  d_skyboxRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    friend class UILayer;

public:
    WorldLayer(Sprocket::Accessor& accessor);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};