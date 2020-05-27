#include "PhysicsEngine.h"
#include "Components.h"

#include <variant>

#include "reactphysics3d.h"

namespace Sprocket {
namespace {

rp3d::Vector3 convert(const Maths::vec3& v)
{
    return rp3d::Vector3(v.x, v.y, v.z);
}

rp3d::Vector2 convert(const Maths::vec2& v)
{
    return rp3d::Vector2(v.x, v.y);
}

Maths::vec3 convert(const rp3d::Vector3& v)
{
    return Maths::vec3(v.x, v.y, v.z);
}

Maths::vec2 convert(const rp3d::Vector2& v)
{
    return Maths::vec2(v.x, v.y);
}

Maths::quat convert(const rp3d::Quaternion& q)
{
    return Maths::quat(q.w, q.x, q.y, q.z);
}

rp3d::Quaternion convert(const Maths::quat& q)
{
    return rp3d::Quaternion(q.x, q.y, q.z, q.w);
}

rp3d::Transform transform(const Entity& entity)
{
    rp3d::Transform t;
    t.setPosition(convert(entity.position()));
    t.setOrientation(convert(entity.orientation()));
    return t;
}

class RaycastCB : public rp3d::RaycastCallback
{
    Entity* d_entity = nullptr;
    float d_fraction = 10.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < d_fraction) {  // This object is closer.
            d_fraction = info.hitFraction;
            d_entity = reinterpret_cast<Entity*>(info.body->getUserData());
        }
        return -1.0f;
    }

    Entity* entity() const { return d_entity; }
    float fraction() const { return d_fraction; }
};

class EventListener : public rp3d::EventListener
{
public:
    void rigidBodyUpdated(const rp3d::RigidBody* body) override
    {
        Entity* entity = reinterpret_cast<Entity*>(body->getUserData());
        entity->position() = convert(body->getTransform().getPosition());
        entity->orientation() = convert(body->getTransform().getOrientation());

        if (entity->has<PhysicsComponent>()) {
            entity->get<PhysicsComponent>().velocity = convert(body->getLinearVelocity());
        }
    }
};

float getSpeed(SpeedFactor s)
{
    switch (s) {
        case SpeedFactor::QUARTER:   return 0.25f;
        case SpeedFactor::HALF:      return 0.5f;
        case SpeedFactor::NORMAL:    return 1.0f;
        case SpeedFactor::DOUBLE:    return 2.0f;
        case SpeedFactor::QUADRUPLE: return 4.0f;
    }

    SPKT_LOG_ERROR("Speed not found! Returning 1.0f");
    return 1.0f;
};

}

struct ColliderData
{
    std::shared_ptr<rp3d::CollisionShape> collisionShape;
    rp3d::ProxyShape*                     proxyShape;
};

struct PhysicsEngineImpl
{
    rp3d::DynamicsWorld world;

    std::unordered_map<
        std::size_t, ColliderData
    > collisionShapes;
        // This is just to manage the lifetimes of the collision bodies. 
        // May want to enhance this in the future for some optimising.

    std::unordered_map<
        std::size_t, rp3d::RigidBody*
    > rigidBodies; 
        // Lifetime of RidigBody managed by RapidPhysics3D?

    EventListener eventListener;

    PhysicsEngineImpl(const Maths::vec3& gravity)
        : world(convert(gravity))
    {}
};

PhysicsEngine::PhysicsEngine(const Maths::vec3& gravity)
    : d_impl(std::make_shared<PhysicsEngineImpl>(gravity))
    , d_timeStep(1.0f / 120.0f)
    , d_lastFrameLength(0)
    , d_speedFactor(SpeedFactor::NORMAL)
    , d_running(true)
{
    d_impl->world.setNbIterationsPositionSolver(5);
    d_impl->world.setNbIterationsVelocitySolver(8);
    d_impl->world.setEventListener(&d_impl->eventListener);
}

void PhysicsEngine::updateSystem(float dt)
{
    if (!d_running) {
        return;
    }

    float speedFactor = getSpeed(d_speedFactor);

    float frameLength = dt * speedFactor;
    d_lastFrameLength = 0;

    static float accumulator = 0.0f;
    accumulator += frameLength;

    // First update the Physics World.
    while (accumulator >= d_timeStep) {
        d_impl->world.update(d_timeStep);
        accumulator -= d_timeStep;
        d_lastFrameLength += d_timeStep;
    }
}

void PhysicsEngine::updateEntity(float dt, Entity& entity)
{
    if (!d_running) {
        return;
    }

    auto bodyData = d_impl->rigidBodies[entity.id()];

    if (!bodyData) {
        return; // This entity is not registered with the physics engine
    }
    
    // Update the RigidBody corresponding to this Entity.
    bodyData->setTransform(transform(entity));

    if (entity.has<PhysicsComponent>()) {
        const auto& physics = entity.get<PhysicsComponent>();

        bodyData->setLinearVelocity(convert(physics.velocity));
        bodyData->enableGravity(physics.gravity);    

        if (physics.frozen) {
            bodyData->setType(rp3d::BodyType::STATIC);
        }
        else {
            bodyData->setType(rp3d::BodyType::DYNAMIC);
        }
    }
    else {
        bodyData->setType(rp3d::BodyType::STATIC);
    }
    
    if (entity.has<ColliderComponent>()) {
        const auto& collider = entity.get<ColliderComponent>();

        bodyData->setMass(collider.mass);

        auto& material = bodyData->getMaterial();
        material.setBounciness(collider.bounciness);
        material.setFrictionCoefficient(collider.frictionCoefficient);
        material.setRollingResistance(collider.rollingResistance); 
    }

    // Handle player movement updates.
    if (entity.has<PlayerComponent>()) {
        updatePlayer(dt, entity);
    }
}

void PhysicsEngine::addCollider(const Entity& entity)
{
    auto& colliderData = entity.get<ColliderComponent>();

    std::shared_ptr<rp3d::CollisionShape> collider = nullptr;
    if (auto data = std::get_if<BoxCollider>(&colliderData.collider)) {
        collider = std::make_shared<rp3d::BoxShape>(convert(data->halfExtents));
    }
    else if (auto data = std::get_if<SphereCollider>(&colliderData.collider)) {
        collider = std::make_shared<rp3d::SphereShape>(data->radius);
    }
    else if (auto data = std::get_if<CapsuleCollider>(&colliderData.collider)) {
        collider = std::make_shared<rp3d::CapsuleShape>(
            data->radius, data->height
        );
    }
    else {
        SPKT_LOG_ERROR("Entity has a ColliderComponent but no collider!");
        return; // No collision data.
    }

    auto& entry = d_impl->rigidBodies[entity.id()];
    auto ps = entry->addCollisionShape(
        collider.get(),
        rp3d::Transform::identity(),
        colliderData.mass
    );

    ColliderData data;
    data.collisionShape = collider;
    data.proxyShape = ps;
    d_impl->collisionShapes[entity.id()] = data;
}

void PhysicsEngine::registerEntity(const Entity& entity)
{
    auto hasPhysics = entity.has<PhysicsComponent>();
    auto hasCollider = entity.has<ColliderComponent>();

    if (!hasPhysics && !hasCollider) {
        return;  // Entity must have physics or collider.
    }

    auto& entry = d_impl->rigidBodies[entity.id()];
    entry = d_impl->world.createRigidBody(transform(entity));

    // Give each RigidBody a pointer back to the original Entity object.
    entry->setUserData(const_cast<void*>(reinterpret_cast<const void*>(&entity)));
    if (hasPhysics) {
        entry->setType(rp3d::BodyType::DYNAMIC);
    }
    else {
        entry->setType(rp3d::BodyType::STATIC);
    }

    if (entity.has<PlayerComponent>()) {
        entry->setAngularDamping(0.0f);
    }

    if (hasCollider) {
        addCollider(entity); // No Collider to add.
    }
}

void PhysicsEngine::onComponentAttach(const Entity& entity, const Component& component)
{
    if (auto coll = dynamic_cast<const ColliderComponent*>(&component)) {
        if (entity.has<PhysicsComponent>()) {
            // Existing entity
            addCollider(entity);
        }
        else {
            // New entity
            registerEntity(entity);
        }
    }
    else if (auto phys = dynamic_cast<const PhysicsComponent*>(&component)) {
        if (entity.has<ColliderComponent>()) {
            // Existing entity
            auto& entry = d_impl->rigidBodies[entity.id()];
            entry->setType(rp3d::BodyType::DYNAMIC);
        }
        else {
            // New entity
            registerEntity(entity);
        }
    }
    // TODO: Handle adding player components
}

void PhysicsEngine::onComponentDetach(const Entity& entity, const Component& component)
{
    if (auto coll = dynamic_cast<const ColliderComponent*>(&component)) {
        if (entity.has<PhysicsComponent>()) {
            // Now has physics but no collider
            auto it = d_impl->collisionShapes.find(entity.id());
            if (it != d_impl->collisionShapes.end()) {
                auto& entry = d_impl->rigidBodies[entity.id()];
                entry->removeCollisionShape(it->second.proxyShape);
            } else {
                SPKT_LOG_ERROR("Tried to remove a collider from a shape with no collider!");
            }
        }
        else {
            // Remove from physics engine
            deregisterEntity(entity);
        }
    }
    else if (auto phys = dynamic_cast<const PhysicsComponent*>(&component)) {
        if (entity.has<ColliderComponent>()) {
            // Now has collider but no physics
            auto& entry = d_impl->rigidBodies[entity.id()];
            entry->setType(rp3d::BodyType::STATIC);
        }
        else {
            // Remove from physics engine
            deregisterEntity(entity);
        }
    }
}

void PhysicsEngine::deregisterEntity(const Entity& entity)
{
    auto hasPhysics = entity.has<PhysicsComponent>();
    auto hasCollider = entity.has<ColliderComponent>();

    if (!hasPhysics && !hasCollider) {
        return;  // Entity must have physics or collider.
    }

    auto rigidBodyIt = d_impl->rigidBodies.find(entity.id());
    if (rigidBodyIt == d_impl->rigidBodies.end()) {
        return;  // Nothing to delete.
    }

    d_impl->world.destroyRigidBody(rigidBodyIt->second);
    d_impl->rigidBodies.erase(rigidBodyIt);
    // TODO: Clean up of collision bodies
}

void PhysicsEngine::running(bool isRunning)
{
    d_running = isRunning;
}

Entity* PhysicsEngine::raycast(const Maths::vec3& base,
                               const Maths::vec3& direction)
{
    Maths::vec3 d = direction;
    Maths::normalise(d);
    d *= 100.0f;

    rp3d::Vector3 start = convert(base);
    rp3d::Vector3 end = start + convert(d);

    rp3d::Ray ray(start, end);
    RaycastCB cb;
    d_impl->world.raycast(ray, &cb);
    return cb.entity();
}

void PhysicsEngine::updatePlayer(float dt, Entity& entity)
{
    if (d_lastFrameLength == 0) {
        return;  // Physics engine not advanced this frame.
    }

    const auto& player = entity.get<PlayerComponent>();
    const auto& physics = entity.get<PhysicsComponent>();

    float mass = d_impl->rigidBodies[entity.id()]->getMass();
        // Sum of all colliders plus rigid body.

    makeUpright(&entity, player.yaw);
    
    bool onFloor = isOnFloor(&entity);

    if (player.direction.length() != 0.0f || onFloor) {
        float speed = 3.0f;
        Maths::vec3 dv = (speed * player.direction) - physics.velocity;
        dv.y = 0.0f;  // Only consider horizontal movement.
        Maths::vec3 acceleration = dv / d_lastFrameLength;
        applyForce(&entity, mass * acceleration);
    }

    // Jumping
    if (onFloor && player.jumping) {
        float speed = 6.0f;
        Maths::vec3 dv = (speed - physics.velocity.y) * Maths::vec3(0, 1, 0);
        Maths::vec3 acceleration = dv / d_lastFrameLength;
        applyForce(&entity, mass * acceleration);
    }
}

void PhysicsEngine::applyForce(Entity* entity, const Maths::vec3& force)
{
    auto& bodyData = d_impl->rigidBodies[entity->id()];
    bodyData->applyForceToCenterOfMass(convert(force));
}

void PhysicsEngine::makeUpright(Entity* entity, float yaw)
{
    auto& bodyData = d_impl->rigidBodies[entity->id()];
    rp3d::Transform transform = bodyData->getTransform();
    rp3d::Quaternion q = rp3d::Quaternion::fromEulerAngles(
        0.0f, Maths::radians(yaw), 0.0f);
    transform.setOrientation(q);
    bodyData->setTransform(transform);
    entity->orientation() = convert(q);
}

bool PhysicsEngine::isOnFloor(const Entity* entity) const
{
    auto& bodyData = d_impl->rigidBodies[entity->id()];

    // Get the point at the bottom of the rigid body.
    auto aabb = bodyData->getAABB();
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
    return cb.entity() != nullptr;
}

void PhysicsEngine::refreshTransform(const Entity* entity)
{
    auto& bodyData = d_impl->rigidBodies[entity->id()];
    bodyData->setTransform(
        rp3d::Transform(
            convert(entity->position()),
            convert(entity->orientation())
        )
    );
}

}