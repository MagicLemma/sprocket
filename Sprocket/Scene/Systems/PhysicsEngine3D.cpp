#include "PhysicsEngine3D.h"
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
    ecs::Entity d_entity = ecs::Null;
    float d_fraction = 10.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < d_fraction) {  // This object is closer.
            d_fraction = info.hitFraction;
            d_entity = *static_cast<ecs::Entity*>(info.body->getUserData());
        }
        return -1.0f;
    }

    ecs::Entity GetEntity() const { return d_entity; }
    float Fraction() const { return d_fraction; }
};

}

struct EntityData
{
    ecs::Entity entity;
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

    float lastFrameLength = 0;

    std::unordered_map<ecs::Entity, EntityData> entityData;

    PhysicsEngine3DImpl(const glm::vec3& gravity)
    {
        rp3d::PhysicsWorld::WorldSettings settings;
        settings.gravity = Convert(gravity);

        world = pc.createPhysicsWorld(settings);
    }

    ~PhysicsEngine3DImpl()
    {
        for (auto& [entity, data] : entityData) {
            world->destroyRigidBody(data.body);
        }
        pc.destroyPhysicsWorld(world);
    }
};

PhysicsEngine3D::PhysicsEngine3D(const glm::vec3& gravity)
    : d_impl(std::make_unique<PhysicsEngine3DImpl>(gravity))
{
}

void PhysicsEngine3D::OnStartup(Scene& scene)
{
    scene.Entities().OnAdd<RigidBody3DComponent>([&](ecs::Entity entity) {
        assert(entity.Has<Transform3DComponent>());
        auto& tc = entity.Get<Transform3DComponent>();
        auto& rc = entity.Get<RigidBody3DComponent>();

        auto& entry = d_impl->entityData[entity];
        entry.entity = entity;
        entry.body = d_impl->world->createRigidBody(Convert(tc));
        entry.body->setUserData(static_cast<void*>(&entry.entity));
    });

    scene.Entities().OnRemove<RigidBody3DComponent>([&](ecs::Entity entity) {
        entity.Remove<BoxCollider3DComponent>();
        entity.Remove<SphereCollider3DComponent>();
        entity.Remove<CapsuleCollider3DComponent>();

        auto rigidBodyIt = d_impl->entityData.find(entity);
        d_impl->world->destroyRigidBody(rigidBodyIt->second.body);
        d_impl->entityData.erase(rigidBodyIt);
    });

    scene.Entities().OnAdd<BoxCollider3DComponent>([&](ecs::Entity entity) {
        assert(entity.Has<Transform3DComponent>());
        assert(entity.Has<RigidBody3DComponent>());

        auto& tc = entity.Get<Transform3DComponent>();
        auto& bc = entity.Get<BoxCollider3DComponent>();
        auto& entry = d_impl->entityData[entity];

        glm::vec3 dimensions = bc.halfExtents;
        if (bc.applyScale) { dimensions *= tc.scale; }
        rp3d::BoxShape* shape = d_impl->pc.createBoxShape(Convert(dimensions));
        rp3d::Transform transform = Convert(bc.position, bc.orientation);

        entry.boxCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.boxCollider, entity.Get<RigidBody3DComponent>());
    });

    scene.Entities().OnRemove<BoxCollider3DComponent>([&](ecs::Entity entity) {
        auto& entry = d_impl->entityData[entity];
        entry.body->removeCollider(entry.boxCollider);
    });

    scene.Entities().OnAdd<SphereCollider3DComponent>([&](ecs::Entity entity) {
        assert(entity.Has<Transform3DComponent>());
        assert(entity.Has<RigidBody3DComponent>());

        auto& tc = entity.Get<Transform3DComponent>();
        auto& sc = entity.Get<SphereCollider3DComponent>();
        auto& entry = d_impl->entityData[entity];
        
        rp3d::SphereShape* shape = d_impl->pc.createSphereShape(sc.radius);
        rp3d::Transform transform = Convert(sc.position, sc.orientation);

        entry.sphereCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.sphereCollider, entity.Get<RigidBody3DComponent>());   
    });

    scene.Entities().OnRemove<SphereCollider3DComponent>([&](ecs::Entity entity) {
        auto& entry = d_impl->entityData[entity];
        entry.body->removeCollider(entry.sphereCollider);
    });

    scene.Entities().OnAdd<CapsuleCollider3DComponent>([&](ecs::Entity entity) {
        assert(entity.Has<Transform3DComponent>());
        assert(entity.Has<RigidBody3DComponent>());

        auto& tc = entity.Get<Transform3DComponent>();
        auto& cc = entity.Get<CapsuleCollider3DComponent>();
        auto& entry = d_impl->entityData[entity];
        
        rp3d::CapsuleShape* shape = d_impl->pc.createCapsuleShape(cc.radius, cc.height);
        rp3d::Transform transform = Convert(cc.position, cc.orientation);

        entry.capsuleCollider = entry.body->addCollider(shape, transform);
        SetMaterial(entry.capsuleCollider, entity.Get<RigidBody3DComponent>()); 
    });

    scene.Entities().OnRemove<CapsuleCollider3DComponent>([&](ecs::Entity entity) {
        auto& entry = d_impl->entityData[entity];
        entry.body->removeCollider(entry.capsuleCollider);
    });
}

void PhysicsEngine3D::OnUpdate(Scene& scene, double dt)
{
    // Pre Update
    // Do this even if not running so that the physics engine stays up
    // to date with the scene.
    for (auto entity : scene.Entities().View<RigidBody3DComponent>()) {
        const auto& tc = entity.Get<Transform3DComponent>();
        const auto& physics = entity.Get<RigidBody3DComponent>();

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
            if (entity.Has<BoxCollider3DComponent>()) { mass += entity.Get<BoxCollider3DComponent>().mass; }
            if (entity.Has<SphereCollider3DComponent>()) { mass += entity.Get<SphereCollider3DComponent>().mass; }
            if (entity.Has<CapsuleCollider3DComponent>()) { mass += entity.Get<CapsuleCollider3DComponent>().mass; }
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
    for (auto entity : scene.Entities().View<RigidBody3DComponent>()) {
        auto& tc = entity.Get<Transform3DComponent>();
        auto& rc = entity.Get<RigidBody3DComponent>();
        const rp3d::RigidBody* body = d_impl->entityData[entity].body;

        tc.position = Convert(body->getTransform().getPosition());
        tc.orientation = Convert(body->getTransform().getOrientation());
        rc.velocity = Convert(body->getLinearVelocity());

        rc.force = {0.0, 0.0, 0.0};
        rc.onFloor = IsOnFloor(entity);
    }
}

ecs::Entity PhysicsEngine3D::Raycast(const glm::vec3& base,
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

bool PhysicsEngine3D::IsOnFloor(ecs::Entity entity) const
{
    // Get the point at the bottom of the rigid body.
    auto aabb = d_impl->entityData[entity].body->getAABB();
    rp3d::Vector3 playerBase = aabb.getCenter();
    playerBase.y = aabb.getMin().y;

    // Raycast down from this point. We actually raycast from slightly
    // higher which works more consistently for some reason. TODO: Find
    // out why
    rp3d::Vector3 up(0.0f, 1.0f, 0.0f);
    float delta = 0.1f;
    rp3d::Ray ray(playerBase + delta * up, playerBase - 2 * delta * up);
    RaycastCB cb;
    d_impl->world->raycast(ray, &cb);
    return cb.GetEntity() != ecs::Null;
}

}
