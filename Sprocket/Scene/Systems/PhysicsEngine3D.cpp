#include "PhysicsEngine3D.h"
#include "ECS.h"
#include "Log.h"
#include "Scene.h"
#include "Components.h"

#include <variant>
#include <unordered_map>

#include <reactphysics3d/reactphysics3d.h>

namespace Sprocket {
namespace {

rp3d::Vector3 Convert(const glm::vec3& v)
{
    return rp3d::Vector3(v.x, v.y, v.z);
}

rp3d::Vector2 Convert(const glm::vec2& v)
{
    return rp3d::Vector2(v.x, v.y);
}

glm::vec3 Convert(const rp3d::Vector3& v)
{
    return glm::vec3(v.x, v.y, v.z);
}

glm::vec2 Convert(const rp3d::Vector2& v)
{
    return glm::vec2(v.x, v.y);
}

glm::quat Convert(const rp3d::Quaternion& q)
{
    return glm::quat(q.w, q.x, q.y, q.z);
}

rp3d::Quaternion Convert(const glm::quat& q)
{
    return rp3d::Quaternion(q.x, q.y, q.z, q.w);
}

rp3d::Transform Convert(const glm::vec3& position, const glm::quat& orientation)
{
    rp3d::Transform t;
    t.setPosition(Convert(position));

    rp3d::Quaternion ori = Convert(orientation);
    ori.normalize();
    t.setOrientation(ori);
    return t;
}

rp3d::Transform Convert(const Transform3DComponent& transform)
{
    return Convert(transform.position, transform.orientation);
}

void SetMaterial(rp3d::Collider* collider, const RigidBody3DComponent& rc)
{
    rp3d::Material& material = collider->getMaterial();
    material.setFrictionCoefficient(rc.frictionCoefficient);
    material.setBounciness(rc.bounciness);
    material.setRollingResistance(rc.rollingResistance);
}

class RaycastCB : public rp3d::RaycastCallback
{
    spkt::entity d_entity = spkt::null;
    float d_fraction = 10.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < d_fraction) {  // This object is closer.
            d_fraction = info.hitFraction;
            d_entity = *static_cast<spkt::entity*>(info.body->getUserData());
        }
        return -1.0f;
    }

    spkt::entity GetEntity() const { return d_entity; }
    float Fraction() const { return d_fraction; }
};

class EventListener : public rp3d::EventListener
{
    std::vector<ev::Event> d_events;

public:
    void onContact(const rp3d::CollisionCallback::CallbackData& data) override
    {
        for (u32 p = 0; p != data.getNbContactPairs(); ++p) {
            auto pair = data.getContactPair(p);
            auto type = pair.getEventType();
            if (type == rp3d::CollisionCallback::ContactPair::EventType::ContactStart) {
                spkt::entity e1 = *static_cast<spkt::entity*>(pair.getBody1()->getUserData());
                spkt::entity e2 = *static_cast<spkt::entity*>(pair.getBody2()->getUserData());
                ev::Event event = ev::make_event<CollisionEvent>(e1, e2);
                d_events.push_back(event);
            }
        }
    }

    std::vector<ev::Event>& events() { return d_events; }
};

}

struct EntityData
{
    spkt::entity entity;
    rp3d::RigidBody* body;

    // Colliders
    rp3d::Collider* boxCollider = nullptr;
    rp3d::Collider* sphereCollider = nullptr;
    rp3d::Collider* capsuleCollider = nullptr;
};

struct PhysicsEngine3DImpl
{
    rp3d::PhysicsCommon pc;
    rp3d::PhysicsWorld* world;

    EventListener listener;

    float lastFrameLength = 0;

    std::unordered_map<spkt::entity, EntityData> entityData;

    PhysicsEngine3DImpl(const glm::vec3& gravity)
    {
        rp3d::PhysicsWorld::WorldSettings settings;
        settings.gravity = Convert(gravity);

        world = pc.createPhysicsWorld(settings);
        world->setEventListener(&listener);
    }

    ~PhysicsEngine3DImpl()
    {
        for (auto& [entity, data] : entityData) {
            world->destroyRigidBody(data.body);
        }
        pc.destroyPhysicsWorld(world);
    }
};

namespace {

bool IsOnFloor(const PhysicsEngine3DImpl& impl, spkt::entity entity)
{
    // Get the point at the bottom of the rigid body.
    auto aabb = impl.entityData.at(entity).body->getAABB();
    rp3d::Vector3 playerBase = aabb.getCenter();
    playerBase.y = aabb.getMin().y;

    // Raycast down from this point. We actually raycast from slightly
    // higher which works more consistently for some reason. TODO: Find
    // out why
    rp3d::Vector3 up(0.0f, 1.0f, 0.0f);
    float delta = 0.1f;
    rp3d::Ray ray(playerBase + delta * up, playerBase - 2 * delta * up);
    RaycastCB cb;
    impl.world->raycast(ray, &cb);
    return cb.GetEntity().valid();
}

}

PhysicsEngine3D::PhysicsEngine3D(const glm::vec3& gravity)
    : d_impl(std::make_unique<PhysicsEngine3DImpl>(gravity))
{
}

void PhysicsEngine3D::on_startup(spkt::registry& registry, ev::Dispatcher& dispatcher)
{
    dispatcher.subscribe<spkt::added<RigidBody3DComponent>>([&](ev::Event& event, auto&& data) {
        assert(data.entity.has<Transform3DComponent>());
        auto& tc = data.entity.get<Transform3DComponent>();

        auto& entry = d_impl->entityData[data.entity];
        entry.entity = data.entity;
        entry.body = d_impl->world->createRigidBody(Convert(tc));
        entry.body->setUserData(static_cast<void*>(&entry.entity));
    });

    dispatcher.subscribe<spkt::removed<RigidBody3DComponent>>([&](ev::Event& event, auto&& data) {
        spkt::entity entity = data.entity;
        entity.remove<BoxCollider3DComponent>();
        entity.remove<SphereCollider3DComponent>();
        entity.remove<CapsuleCollider3DComponent>();

        auto rigidBodyIt = d_impl->entityData.find(data.entity);
        d_impl->world->destroyRigidBody(rigidBodyIt->second.body);
        d_impl->entityData.erase(rigidBodyIt);
    });

    dispatcher.subscribe<spkt::added<BoxCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        assert(data.entity.has<Transform3DComponent>());
        assert(data.entity.has<RigidBody3DComponent>());

        auto& tc = data.entity.get<Transform3DComponent>();
        auto& bc = data.entity.get<BoxCollider3DComponent>();

        auto it = d_impl->entityData.find(data.entity);
        assert(it != d_impl->entityData.end());
        auto& entry = it->second;

        glm::vec3 dimensions = bc.halfExtents;
        if (bc.applyScale) { dimensions *= tc.scale; }
        rp3d::BoxShape* shape = d_impl->pc.createBoxShape(Convert(dimensions));
        rp3d::Transform transform = Convert(bc.position, bc.orientation);

        entry.boxCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.boxCollider, data.entity.get<RigidBody3DComponent>());
    });

    dispatcher.subscribe<spkt::removed<BoxCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        auto& entry = d_impl->entityData[data.entity];
        entry.body->removeCollider(entry.boxCollider);
    });

    dispatcher.subscribe<spkt::added<SphereCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        assert(data.entity.has<Transform3DComponent>());
        assert(data.entity.has<RigidBody3DComponent>());

        auto& tc = data.entity.get<Transform3DComponent>();
        auto& sc = data.entity.get<SphereCollider3DComponent>();
        auto& entry = d_impl->entityData[data.entity];
        
        rp3d::SphereShape* shape = d_impl->pc.createSphereShape(sc.radius);
        rp3d::Transform transform = Convert(sc.position, sc.orientation);

        entry.sphereCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.sphereCollider, data.entity.get<RigidBody3DComponent>());   
    });

    dispatcher.subscribe<spkt::removed<SphereCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        auto& entry = d_impl->entityData[data.entity];
        entry.body->removeCollider(entry.sphereCollider);
    });

    dispatcher.subscribe<spkt::added<CapsuleCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        assert(data.entity.has<Transform3DComponent>());
        assert(data.entity.has<RigidBody3DComponent>());

        auto& tc = data.entity.get<Transform3DComponent>();
        auto& cc = data.entity.get<CapsuleCollider3DComponent>();
        auto& entry = d_impl->entityData[data.entity];
        
        rp3d::CapsuleShape* shape = d_impl->pc.createCapsuleShape(cc.radius, cc.height);
        rp3d::Transform transform = Convert(cc.position, cc.orientation);

        entry.capsuleCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.capsuleCollider, data.entity.get<RigidBody3DComponent>()); 
    });

    dispatcher.subscribe<spkt::removed<CapsuleCollider3DComponent>>([&](ev::Event& event, auto&& data) {
        auto& entry = d_impl->entityData[data.entity];
        entry.body->removeCollider(entry.capsuleCollider);
    });
}

void PhysicsEngine3D::on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt)
{
    // Pre Update
    for (auto id : registry.view<RigidBody3DComponent>()) {
        spkt::entity entity{registry, id};
        const auto& tc = entity.get<Transform3DComponent>();
        const auto& physics = entity.get<RigidBody3DComponent>();

        auto& entry = d_impl->entityData[entity];
        rp3d::RigidBody* body = entry.body;

        body->setTransform(Convert(tc));
        body->setLinearVelocity(Convert(physics.velocity));
        body->enableGravity(physics.gravity);    

        if (physics.frozen) {
            body->setType(rp3d::BodyType::STATIC);
        }
        else {
            body->setType(rp3d::BodyType::DYNAMIC);

            // TODO: Move to RigidBody3DComponent
            float mass = 0;
            if (entity.has<BoxCollider3DComponent>()) { mass += entity.get<BoxCollider3DComponent>().mass; }
            if (entity.has<SphereCollider3DComponent>()) { mass += entity.get<SphereCollider3DComponent>().mass; }
            if (entity.has<CapsuleCollider3DComponent>()) { mass += entity.get<CapsuleCollider3DComponent>().mass; }
            body->setMass(mass);
        }

        if (d_impl->lastFrameLength > 0) {
            auto f = physics.force / d_impl->lastFrameLength;
            body->applyForceToCenterOfMass(Convert(f));
        }
    }
    
    // Update System
    d_impl->lastFrameLength = 0;

    static float accumulator = 0.0f;
    accumulator += static_cast<float>(dt);

    // First update the Physics World.
    while (accumulator >= TIME_STEP) {
        d_impl->world->update(TIME_STEP);
        accumulator -= TIME_STEP;
        d_impl->lastFrameLength += TIME_STEP;
    }

    // Post Update
    for (auto id : registry.view<RigidBody3DComponent>()) {
        spkt::entity entity{registry, id};
        auto& tc = entity.get<Transform3DComponent>();
        auto& rc = entity.get<RigidBody3DComponent>();
        const rp3d::RigidBody* body = d_impl->entityData[entity].body;

        tc.position = Convert(body->getTransform().getPosition());
        tc.orientation = Convert(body->getTransform().getOrientation());
        rc.velocity = Convert(body->getLinearVelocity());

        rc.force = {0.0, 0.0, 0.0};
        rc.onFloor = IsOnFloor(*d_impl, entity);
    }

    // Publish all generated events
    for (auto& event : d_impl->listener.events()) {
        dispatcher.publish(event);
    }
    d_impl->listener.events().clear();
}

spkt::entity PhysicsEngine3D::Raycast(const glm::vec3& base,
                                   const glm::vec3& direction)
{
    glm::vec3 d = glm::normalize(direction);
    d *= 100.0f;

    rp3d::Vector3 start = Convert(base);
    rp3d::Vector3 end = start + Convert(d);

    rp3d::Ray ray(start, end);
    RaycastCB cb;
    d_impl->world->raycast(ray, &cb);
    return cb.GetEntity();
}

}
