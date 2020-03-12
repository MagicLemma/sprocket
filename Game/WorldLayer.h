#pragma once
#include <Sprocket.h>

#include "WorldSceneInfo.h"

#include <memory>
#include <random>

class WorldLayer : public Sprocket::Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;
    Sprocket::SkyboxRenderer  d_skyboxRenderer;

    Sprocket::PostProcessor   d_postProcessor;

public:
    WorldLayer(std::shared_ptr<BasicSceneInfo> info);

    bool handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event) override;
    void updateImpl(Sprocket::Window* window) override;
    void drawImpl(Sprocket::Window* window) override;
};