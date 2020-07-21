#pragma once
#include <Sprocket.h>

#include <queue>

struct PathComponent : public Sprocket::Component
{
    std::queue<Sprocket::Maths::vec3> markers;
    float                             speed;
};

class PathFollower : public Sprocket::EntitySystem
{
public:
    void UpdateEntity(double dt, Sprocket::Entity& entity) override;
        // Called once per entity per frame and before the system updates.
};