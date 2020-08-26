#include "PhysicsEngine.h"
#include "Log.h"
#include "Scene.h"
#include "Components.h"

#include <variant>

#include "reactphysics3d.h"

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

rp3d::Transform Convert(const TransformComponent& transform)
{
    rp3d::Transform t;
    t.setPosition(Convert(transform.position));
    t.setOrientation(Convert(transform.orientation));
    return t;
}

class RaycastCB : public rp3d::RaycastCallback
{
    Entity d_entity = Entity();
    float d_fraction = 10.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < d_fraction) {  // This object is closer.
            d_fraction = info.hitFraction;
            d_entity = *static_cast<Entity*>(info.body->getUserData());
        }
        return -1.0f;
    }

    Entity GetEntity() const { return d_entity; }
    float Fraction() const { return d_fraction; }
};

}

struct EntityData
{
    Entity                                entity;
    rp3d::RigidBody*                      rigidBody;

    // Box
    rp3d::ProxyShape*                     boxProxyShape     = nullptr;
    std::shared_ptr<rp3d::CollisionShape> boxCollisionShape = nullptr;

    // Sphere
    rp3d::ProxyShape*                     sphereProxyShape     = nullptr;
    std::shared_ptr<rp3d::CollisionShape> sphereCollisionShape = nullptr;

    // Capsule
    rp3d::ProxyShape*                     capsuleProxyShape     = nullptr;
    std::shared_ptr<rp3d::CollisionShape> capsuleCollisionShape = nullptr;
};

struct PhysicsEngineImpl
{
    rp3d::DynamicsWorld world;

    std::unordered_map<uint32_t, EntityData> entityData;

    PhysicsEngineImpl(const Maths::vec3& gravity)
        : world(Convert(gravity))
    {}
};

PhysicsEngine::PhysicsEngine(const Maths::vec3& gravity)
    : d_impl(std::make_shared<PhysicsEngineImpl>(gravity))
    , d_timeStep(1.0f / 120.0f)
    , d_lastFrameLength(0)
    , d_running(true)
{
    d_impl->world.setNbIterationsPositionSolver(5);
    d_impl->world.setNbIterationsVelocitySolver(8);
}

void PhysicsEngine::OnStartup(Scene& scene)
{
    auto addPhysics = [&](Entity& entity) {
        assert(entity.Has<TransformComponent>());
        auto& transform = entity.Get<TransformComponent>();

        auto& entry = d_impl->entityData[entity.Id()];
        entry.entity = entity;
        entry.rigidBody = d_impl->world.createRigidBody(Convert(transform));
        entry.rigidBody->setUserData(static_cast<void*>(&entry.entity));
    };

    scene.Each<RigidBody3DComponent>(addPhysics);
    scene.OnAdd<RigidBody3DComponent>(addPhysics);

    scene.OnRemove<RigidBody3DComponent>([&](Entity& entity) {
        entity.Remove<BoxCollider3DComponent>();
        entity.Remove<SphereCollider3DComponent>();
        entity.Remove<CapsuleCollider3DComponent>();

        auto rigidBodyIt = d_impl->entityData.find(entity.Id());
        d_impl->world.destroyRigidBody(rigidBodyIt->second.rigidBody);
        d_impl->entityData.erase(rigidBodyIt);
    });

    auto addBox = [&](Entity& entity) {
        assert(entity.Has<TransformComponent>());
        assert(entity.Has<RigidBody3DComponent>());
        auto& transform = entity.Get<TransformComponent>();
        auto& box = entity.Get<BoxCollider3DComponent>();

        std::shared_ptr<rp3d::CollisionShape> collider;
        collider = std::make_shared<rp3d::BoxShape>(Convert(box.halfExtents));

        auto entry = d_impl->entityData[entity.Id()].rigidBody;
        d_impl->entityData[entity.Id()].boxProxyShape = entry->addCollisionShape(
            collider.get(),
            rp3d::Transform::identity(),
            box.mass
        );
        d_impl->entityData[entity.Id()].boxCollisionShape = collider;
    };

    scene.Each<BoxCollider3DComponent>(addBox);
    scene.OnAdd<BoxCollider3DComponent>(addBox);

    scene.OnRemove<BoxCollider3DComponent>([&](Entity& entity) {
        auto c = d_impl->entityData[entity.Id()].boxProxyShape;
        if (c != nullptr) {
            d_impl->entityData[entity.Id()].rigidBody->removeCollisionShape(c);
            d_impl->entityData[entity.Id()].boxCollisionShape = nullptr;
        }
    });

    auto addSphere = [&](Entity& entity) {
        assert(entity.Has<TransformComponent>());
        assert(entity.Has<RigidBody3DComponent>());
        auto& transform = entity.Get<TransformComponent>();
        auto& sphere = entity.Get<SphereCollider3DComponent>();

        std::shared_ptr<rp3d::CollisionShape> collider;
        collider = std::make_shared<rp3d::SphereShape>(sphere.radius);

        auto entry = d_impl->entityData[entity.Id()].rigidBody;
        d_impl->entityData[entity.Id()].sphereProxyShape = entry->addCollisionShape(
            collider.get(),
            rp3d::Transform::identity(),
            sphere.mass
        );
        d_impl->entityData[entity.Id()].sphereCollisionShape = collider;
    };

    scene.Each<SphereCollider3DComponent>(addSphere);
    scene.OnAdd<SphereCollider3DComponent>(addSphere);

    scene.OnRemove<SphereCollider3DComponent>([&](Entity& entity) {
        auto c = d_impl->entityData[entity.Id()].sphereProxyShape;
        if (c != nullptr) {
            d_impl->entityData[entity.Id()].rigidBody->removeCollisionShape(c);
            d_impl->entityData[entity.Id()].sphereCollisionShape = nullptr;
        }
    });

    auto addCapsule = [&](Entity& entity) {
        assert(entity.Has<TransformComponent>());
        assert(entity.Has<RigidBody3DComponent>());
        auto& transform = entity.Get<TransformComponent>();
        auto& capsule = entity.Get<CapsuleCollider3DComponent>();

        std::shared_ptr<rp3d::CollisionShape> collider;
        collider = std::make_shared<rp3d::CapsuleShape>(
            capsule.radius, capsule.height
        );

        auto entry = d_impl->entityData[entity.Id()].rigidBody;
        d_impl->entityData[entity.Id()].capsuleProxyShape = entry->addCollisionShape(
            collider.get(),
            rp3d::Transform::identity(),
            capsule.mass
        );
        d_impl->entityData[entity.Id()].capsuleCollisionShape = collider;
    };

    scene.Each<CapsuleCollider3DComponent>(addCapsule);
    scene.OnAdd<CapsuleCollider3DComponent>(addCapsule);

    scene.OnRemove<CapsuleCollider3DComponent>([&](Entity& entity) {
        auto c = d_impl->entityData[entity.Id()].capsuleProxyShape;
        if (c != nullptr) {
            d_impl->entityData[entity.Id()].rigidBody->removeCollisionShape(c);
            d_impl->entityData[entity.Id()].capsuleCollisionShape = nullptr;
        }
    });
}

void PhysicsEngine::OnUpdate(Scene& scene, double dt)
{
    // Pre Update
    // Do this even if not running so that the physics engine stays up
    // to date with the scene.
    scene.Each<TransformComponent, RigidBody3DComponent>([&] (Entity& entity) {
        const auto& transform = entity.Get<TransformComponent>();
        const auto& physics = entity.Get<RigidBody3DComponent>();

        auto& entry = d_impl->entityData[entity.Id()];
        auto bodyData = entry.rigidBody;
        
        bodyData->setTransform(Convert(transform));
        bodyData->setLinearVelocity(Convert(physics.velocity));
        bodyData->enableGravity(physics.gravity);    
        bodyData->setType(physics.frozen ? rp3d::BodyType::STATIC : rp3d::BodyType::DYNAMIC);

        auto& material = bodyData->getMaterial();
        material.setBounciness(physics.bounciness);
        material.setFrictionCoefficient(physics.frictionCoefficient);
        material.setRollingResistance(physics.rollingResistance);

        if (d_lastFrameLength > 0) {
            auto f = physics.force / d_lastFrameLength;
            bodyData->applyForceToCenterOfMass(Convert(f));
        }
    });

    if (!d_running) { return; }
    
    // Update System
    d_lastFrameLength = 0;

    static float accumulator = 0.0f;
    accumulator += dt;

    // First update the Physics World.
    while (accumulator >= d_timeStep) {
        d_impl->world.update(d_timeStep);
        accumulator -= d_timeStep;
        d_lastFrameLength += d_timeStep;
    }

    // Post Update
    scene.Each<TransformComponent, RigidBody3DComponent>([&] (Entity& entity) {
        auto& transform = entity.Get<TransformComponent>();
        auto& physics = entity.Get<RigidBody3DComponent>();
        const auto& bodyData = d_impl->entityData[entity.Id()].rigidBody;

        transform.position = Convert(bodyData->getTransform().getPosition());
        transform.orientation = Convert(bodyData->getTransform().getOrientation());
        physics.velocity = Convert(bodyData->getLinearVelocity());

        physics.force = {0.0, 0.0, 0.0};
        physics.onFloor = IsOnFloor(entity);
    });
}

void PhysicsEngine::Running(bool isRunning)
{
    d_running = isRunning;
}

Entity PhysicsEngine::Raycast(const Maths::vec3& base,
                              const Maths::vec3& direction)
{
    Maths::vec3 d = direction;
    Maths::Normalise(d);
    d *= 100.0f;

    rp3d::Vector3 start = Convert(base);
    rp3d::Vector3 end = start + Convert(d);

    rp3d::Ray ray(start, end);
    RaycastCB cb;
    d_impl->world.raycast(ray, &cb);
    return cb.GetEntity();
}

bool PhysicsEngine::IsOnFloor(Entity entity) const
{
    // Get the point at the bottom of the rigid body.
    auto aabb = d_impl->entityData[entity.Id()].rigidBody->getAABB();
    rp3d::Vector3 playerBase = aabb.getCenter();
    playerBase.y = aabb.getMin().y;

    // Raycast down from this point. We actually raycast from slightly
    // higher which works more consistently for some reason. TODO: Find
    // out why
    rp3d::Vector3 up(0.0f, 1.0f, 0.0f);
    float delta = 0.1f;
    rp3d::Ray ray(playerBase + delta * up, playerBase - 2 * delta * up);
    RaycastCB cb;
    d_impl->world.raycast(ray, &cb);
    return !cb.GetEntity().Null();
}

}
