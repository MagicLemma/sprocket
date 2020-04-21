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
    d_impl->world.enableSleeping(false);
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

void PhysicsEngine::updateEntity(Entity& entity)
{
    if (!d_running) {
        return;
    }

    if (entity.hasComponent<PhysicsComponent>() &&
        entity.hasComponent<TransformComponent>()) {
        
        auto& t = entity.getComponent<TransformComponent>();
        auto& physics = entity.getComponent<PhysicsComponent>();

        const auto& bodyData = d_impl->rigidBodies[entity.id()];

        if (physics.stationary) {
            rp3d::Transform transform;
            transform.setFromOpenGL(&t.transform[0][0]);
            bodyData->setTransform(transform);
        }
        else {
            bodyData->getTransform().getOpenGLMatrix(&t.transform[0][0]);
        }
    }
}

void PhysicsEngine::registerEntity(const Entity& entity)
{
    if (!entity.hasComponent<PhysicsComponent>()) {
        return;  // Entity must have physics.
    }

    auto& physicsData = entity.getComponent<PhysicsComponent>();
    auto& entry = d_impl->rigidBodies[entity.id()];

    rp3d::Transform transform = rp3d::Transform::identity();
    if (entity.hasComponent<TransformComponent>()) {
        Maths::mat4& entityTransform = entity.getComponent<TransformComponent>().transform; 
        transform.setFromOpenGL(&entityTransform[0][0]);
    }
    
    entry = d_impl->world.createRigidBody(transform);
    entry->setType(
        physicsData.stationary ?
            rp3d::BodyType::STATIC : rp3d::BodyType::DYNAMIC
    );

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

    entry->addCollisionShape(
        collider.get(),
        rp3d::Transform::identity(),
        physicsData.mass
    );

    d_impl->collisionShapes.push_back(collider);
}

void PhysicsEngine::deregisterEntity(const Entity& entity)
{
    if (!entity.hasComponent<PhysicsComponent>()) {
        // Entity must have physics.
        return; 
    }

    // TODO
}

void PhysicsEngine::running(bool isRunning)
{
    d_running = isRunning;
}

}