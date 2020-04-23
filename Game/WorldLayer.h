#pragma once
#include <Sprocket.h>

#include <memory>
#include <random>

class WorldLayer : public Sprocket::Layer
{
    Sprocket::FirstPersonCamera d_firstCamera;
    Sprocket::PlayerBasedCamera d_playerCamera;
    Sprocket::Camera* d_camera;
    bool d_cameraIsFirst = true;

    Sprocket::PerspectiveLens d_lens;

    Sprocket::Skybox d_skybox;
 
    Sprocket::EntityManager  d_entityManager;
    Sprocket::PlayerMovement d_playerMovement;
    Sprocket::PhysicsEngine  d_physicsEngine;
    
    std::vector<Sprocket::Light>   d_lights;
    
    bool d_paused = false;

    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;
    Sprocket::SkyboxRenderer  d_skyboxRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    bool d_run = false;

    friend class UILayer;

public:
    WorldLayer(Sprocket::Accessor& accessor);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};