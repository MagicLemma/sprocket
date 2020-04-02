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
    WorldLayer(Sprocket::Accessor& accessor, std::shared_ptr<BasicSceneInfo> info);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};