#include "Physics.h"
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

Maths::vec4 convert(const rp3d::Quaternion& v)
{
    return Maths::vec4(v.x, v.y, v.z, v.w);
}

Maths::vec3 convert(const rp3d::Vector3& v)
{
    return Maths::vec3(v.x, v.y, v.z);
}

Maths::vec2 convert(const rp3d::Vector2& v)
{
    return Maths::vec2(v.x, v.y);
}

Maths::mat3 convert(const rp3d::Matrix3x3& m)
{
    Maths::mat3 ret;
    ret[0][0] = m[0][0];
    ret[1][0] = m[1][0];
    ret[2][0] = m[2][0];

    ret[0][1] = m[0][1];
    ret[1][1] = m[1][1];
    ret[2][1] = m[2][1];

    ret[0][2] = m[0][2];
    ret[1][2] = m[1][2];
    ret[2][2] = m[2][2];
    return ret;
}

rp3d::Matrix3x3 convert(const Maths::mat4& m)
{
    rp3d::Matrix3x3 ret;
    ret[0][0] = m[0][0];
    ret[1][0] = m[1][0];
    ret[2][0] = m[2][0];

    ret[0][1] = m[0][1];
    ret[1][1] = m[1][1];
    ret[2][1] = m[2][1];

    ret[0][2] = m[0][2];
    ret[1][2] = m[1][2];
    ret[2][2] = m[2][2];
    return ret;
}

class RaycastCB : public rp3d::RaycastCallback
{
    Entity* d_entity = nullptr;
    float d_fraction = -1.0f;

public:
    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        d_entity = reinterpret_cast<Entity*>(info.body->getUserData());
        d_fraction = info.hitFraction;
        return 0.0;  // Store the first entity hit.
    }

    Entity* entity() const { return d_entity; }
    float fraction() const { return d_fraction; }
};

}

struct PhysicsEngineImpl
{
    rp3d::DynamicsWorld world;

    std::vector<std::shared_ptr<rp3d::CollisionShape>> collisionShapes;
        // This is just to manage the lifetimes of the collision bodies
        // and should really not be touched. May want to enhance this in the
        // future for some optimising.

    std::unordered_map<std::size_t, rp3d::RigidBody*> rigidBodies; 
        // Lifetime of RidigBody managed by RapidPhysics3D?

    PhysicsEngineImpl(const Maths::vec3& gravity)
        : world(convert(gravity))
    {}
};

PhysicsEngine::PhysicsEngine(const Maths::vec3& gravity)
    : d_timeStep(1.0f / 60.0f)
    , d_impl(std::make_shared<PhysicsEngineImpl>(gravity))
{
    d_impl->world.setNbIterationsPositionSolver(5);
    d_impl->world.setNbIterationsVelocitySolver(8);
}

void PhysicsEngine::updateSystem(float dt)
{
    if (!d_running) {
        return;
    }

    static float accumulator = 0.0f;
    accumulator += dt;

    // First update the Physics World.
    while (accumulator >= d_timeStep) {
        d_impl->world.update(d_timeStep);
        accumulator -= d_timeStep;
    }
}

void PhysicsEngine::preUpdateEntity(Entity& entity, float dt)
{
    if (!d_running) {
        return;
    }

    if (entity.has<PhysicsComponent>() &&
        entity.has<TransformComponent>()) {
    
        const auto& t = entity.get<TransformComponent>();
        const auto& physics = entity.get<PhysicsComponent>();

        auto bodyData = d_impl->rigidBodies[entity.id()];

        // Update the RigidBody corresponding to this Entity.
        rp3d::Transform transform = bodyData->getTransform();
        transform.setPosition(convert(t.position));
        if (physics.stationary) {
            transform.setOrientation(rp3d::Quaternion(convert(t.orientation).getTranspose()));
            // Only do this for static bodies as updating this behaves
            // weirdly for dynamic ones.
        }
        bodyData->setTransform(transform);
        bodyData->setMass(physics.mass);
        bodyData->setLinearVelocity(convert(physics.velocity));
        bodyData->enableGravity(physics.gravity);

        auto& material = bodyData->getMaterial();
        material.setBounciness(physics.bounciness);
        material.setFrictionCoefficient(physics.frictionCoefficient);
        material.setRollingResistance(physics.rollingResistance);        
    }
}

void PhysicsEngine::postUpdateEntity(Entity& entity, float dt)
{
    if (!d_running) {
        return;
    }

    if (entity.has<PhysicsComponent>() && entity.has<TransformComponent>()) {
        
        auto& t = entity.get<TransformComponent>();
        auto& physics = entity.get<PhysicsComponent>();

        const auto& bodyData = d_impl->rigidBodies[entity.id()];

        // Update the Entity corresponding to this RigidBody.
        if (!physics.stationary) {
            auto tr = bodyData->getTransform();
            t.position = convert(tr.getPosition());
            t.orientation = convert(tr.getOrientation().getMatrix().getTranspose());
                // We transpose here as OpenGL uses column major ordering
        }

        physics.mass = bodyData->getMass();
        physics.velocity = convert(bodyData->getLinearVelocity());

        auto material = bodyData->getMaterial();
        physics.bounciness = material.getBounciness();
        physics.frictionCoefficient = material.getFrictionCoefficient();
        physics.rollingResistance = material.getRollingResistance();

        // Handle player movement updates.
        if (entity.has<PlayerComponent>()) {
            updatePlayer(entity, dt);
        }
    }

}

void PhysicsEngine::registerEntity(const Entity& entity)
{
    if (!entity.has<PhysicsComponent>()) {
        return;  // Entity must have physics.
    }

    auto& physicsData = entity.get<PhysicsComponent>();
    auto& entry = d_impl->rigidBodies[entity.id()];

    rp3d::Transform transform = rp3d::Transform::identity();
    if (entity.has<TransformComponent>()) {
        auto transformData = entity.get<TransformComponent>();
        transform.setPosition(convert(transformData.position));
        transform.setOrientation(rp3d::Quaternion(convert(transformData.orientation)));
    }
    
    entry = d_impl->world.createRigidBody(transform);

    // Give each RigidBody a pointer back to the original Entity object.
    entry->setUserData(const_cast<void*>(reinterpret_cast<const void*>(&entity)));
    if (physicsData.stationary) {
        entry->setType(rp3d::BodyType::STATIC);
    }
    else {
        entry->setType(rp3d::BodyType::DYNAMIC);
    }

    if (entity.has<PlayerComponent>()) {
        entry->setAngularDamping(0.0f);
    }

    std::shared_ptr<rp3d::CollisionShape> collider = nullptr;
    if (auto data = std::get_if<BoxCollider>(&physicsData.collider)) {
        collider = std::make_shared<rp3d::BoxShape>(convert(data->halfExtents));
    }
    else if (auto data = std::get_if<SphereCollider>(&physicsData.collider)) {
        collider = std::make_shared<rp3d::SphereShape>(data->radius);
    }
    else if (auto data = std::get_if<CapsuleCollider>(&physicsData.collider)) {
        collider = std::make_shared<rp3d::CapsuleShape>(
            data->radius, data->height
        );
    }
    else {
        return; // No collision data.
    }

    auto ps = entry->addCollisionShape(
        collider.get(),
        rp3d::Transform::identity(),
        physicsData.mass
    );

    d_impl->collisionShapes.push_back(collider);
}

void PhysicsEngine::deregisterEntity(const Entity& entity)
{
    if (!entity.has<PhysicsComponent>()) {
        // Entity must have physics.
        return; 
    }

    // TODO
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
    d *= 1000.0f;

    rp3d::Vector3 start = convert(base);
    rp3d::Vector3 end = start + convert(d);

    rp3d::Ray ray(start, end);
    RaycastCB cb;
    d_impl->world.raycast(ray, &cb);
    return cb.entity();
}

void PhysicsEngine::updatePlayer(Entity& entity, float dt)
{
    auto& bodyData = d_impl->rigidBodies[entity.id()];
    
    const auto& player = entity.get<PlayerComponent>();
    const auto& physics = entity.get<PhysicsComponent>();

    // Raycast down to see if there is an object underneath
    auto aabb = bodyData->getAABB();
    rp3d::Vector3 playerBase = aabb.getCenter();
    playerBase.y = aabb.getMin().y;

    // Keep player upright
    rp3d::Transform transform = bodyData->getTransform();
    rp3d::Quaternion q = rp3d::Quaternion::fromEulerAngles(
        0.0f, Maths::radians(player.yaw), 0.0f);
    transform.setOrientation(q);
    bodyData->setTransform(transform);

    // Player Movement
    float cosYaw = Maths::cosd(player.yaw);
    float sinYaw = Maths::sind(player.yaw);
    rp3d::Vector3 forwards(-sinYaw, 0, -cosYaw);
    rp3d::Vector3 right(cosYaw, 0, -sinYaw);

    rp3d::Vector3 direction;
    if (player.movingForwards) { direction += forwards; }
    if (player.movingBackwards) { direction -= forwards; }
    if (player.movingRight) { direction += right; }
    if (player.movingLeft) { direction -= right; }
    direction.normalize();

    float speed = 6.0f * 1000.0f * dt;
    rp3d::Vector3 velChange = (speed * direction) - convert(physics.velocity);
    velChange.y = 0.0f;  // Only consider horizontal movement.

    bodyData->applyForceToCenterOfMass(bodyData->getMass() * velChange);

    // Jumping
    rp3d::Vector3 delta(0.0f, 0.1f, 0.0f);
    rp3d::Ray ray(playerBase + delta, playerBase - 2 * delta);
    RaycastCB cb;
    d_impl->world.raycast(ray, &cb);

    if (cb.entity() && player.jumping) {
        float jumpForce = 35.0f * 10000.0f * dt;
        bodyData->applyForceToCenterOfMass(rp3d::Vector3(0, jumpForce, 0));
    }
}

}