#pragma once
#include <Sprocket.h>

#include <queue>
#include <map>

struct PathComponent
{
    std::queue<Sprocket::Maths::vec3> markers;
    float                             speed;
};

class PathFollower : public Sprocket::EntitySystem
{
public:
    void OnUpdate(std::map<entt::entity, Sprocket::Entity>& entities, double dt) override;
        // Called once per entity per frame and before the system updates.
};