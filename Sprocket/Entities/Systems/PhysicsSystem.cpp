#include "PhysicsSystem.h"
#include "Maths.h"

#include "reactphysics3d.h"

#if 0
namespace Sprocket {
namespace {

rp3d::Vector3 Convert(const Maths::vec3& v)
{
    return rp3d::Vector3(v.x, v.y, v.z);
}

rp3d::Vector2 Convert(const Maths::vec2& v)
{
    return rp3d::Vector2(v.x, v.y);
}

Maths::vec3 Convert(const rp3d::Vector3& v)
{
    return Maths::vec3(v.x, v.y, v.z);
}

Maths::vec2 Convert(const rp3d::Vector2& v)
{
    return Maths::vec2(v.x, v.y);
}

Maths::quat Convert(const rp3d::Quaternion& q)
{
    return Maths::quat(q.w, q.x, q.y, q.z);
}

rp3d::Quaternion Convert(const Maths::quat& q)
{
    return rp3d::Quaternion(q.x, q.y, q.z, q.w);
}

class RaycastCB : public rp3d::RaycastCallback
{
    entt::entity* d_entity = nullptr;
    float d_fraction = 10.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < d_fraction) {  // This object is closer.
            d_fraction = info.hitFraction;
            d_entity = reinterpret_cast<entt::entity*>(info.body->getUserData());
        }
        return -1.0f;
    }

    entt::entity* EntityPtr() const { return d_entity; }
    float Fraction() const { return d_fraction; }
};

class EventListener : public rp3d::EventListener
{
public:
    void rigidBodyUpdated(const rp3d::RigidBody* body) override
    {
        Entity* entity = reinterpret_cast<Entity*>(body->getUserData());
        auto& tr = entity->Get<TransformComponent>();
        tr.position = Convert(body->getTransform().getPosition());
        tr.orientation = Convert(body->getTransform().getOrientation());

        if (entity->Has<PhysicsComponent>()) {
            entity->Get<PhysicsComponent>().velocity = Convert(body->getLinearVelocity());
        }
    }
};

}
}
#endif


