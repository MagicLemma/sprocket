#include "physics_system.h"

#include <Sprocket/Scene/Scene.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <variant>

namespace spkt {
namespace {

rp3d::Vector3 convert(const glm::vec3& v)
{
    return rp3d::Vector3(v.x, v.y, v.z);
}

rp3d::Vector2 convert(const glm::vec2& v)
{
    return rp3d::Vector2(v.x, v.y);
}

glm::vec3 convert(const rp3d::Vector3& v)
{
    return glm::vec3(v.x, v.y, v.z);
}

glm::vec2 convert(const rp3d::Vector2& v)
{
    return glm::vec2(v.x, v.y);
}

glm::quat convert(const rp3d::Quaternion& q)
{
    return glm::quat(q.w, q.x, q.y, q.z);
}

rp3d::Quaternion convert(const glm::quat& q)
{
    return rp3d::Quaternion(q.x, q.y, q.z, q.w);
}

rp3d::Transform convert(const glm::vec3& position, const glm::quat& orientation)
{
    rp3d::Transform t;
    t.setPosition(convert(position));

    rp3d::Quaternion ori = convert(orientation);
    ori.normalize();
    t.setOrientation(ori);
    return t;
}

rp3d::Transform convert(const Transform3DComponent& transform)
{
    return convert(transform.position, transform.orientation);
}

void set_material(rp3d::Collider* collider, const RigidBody3DComponent& rc)
{
    rp3d::Material& material = collider->getMaterial();
    material.setFrictionCoefficient(rc.frictionCoefficient);
    material.setBounciness(rc.bounciness);
    material.setRollingResistance(rc.rollingResistance);
}

struct raycast_callback : public rp3d::RaycastCallback
{
    spkt::entity entity = spkt::null;
    float fraction = 10.0f;

    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override 
    {
        if (info.hitFraction < fraction) {  // This object is closer.
            fraction = info.hitFraction;
            entity = *static_cast<spkt::entity*>(info.body->getUserData());
        }
        return -1.0f;
    }
};

class event_listener : public rp3d::EventListener
{
    using collision_vector = std::vector<std::pair<spkt::entity, spkt::entity>>;

    collision_vector d_collisions;

public:
    void onContact(const rp3d::CollisionCallback::CallbackData& data) override
    {
        for (std::uint32_t p = 0; p != data.getNbContactPairs(); ++p) {
            auto pair = data.getContactPair(p);
            auto type = pair.getEventType();
            if (type == rp3d::CollisionCallback::ContactPair::EventType::ContactStart) {
                spkt::entity e1 = *static_cast<spkt::entity*>(pair.getBody1()->getUserData());
                spkt::entity e2 = *static_cast<spkt::entity*>(pair.getBody2()->getUserData());
                d_collisions.push_back({e1, e2});
            }
        }
    }

    collision_vector& collisions() { return d_collisions; }
};

}

struct physics_runtime
{
    spkt::registry* registry;

    rp3d::PhysicsCommon pc;
    rp3d::PhysicsWorld* world;

    spkt::event_listener listener;

    float lastFrameLength = 0;

    physics_runtime(spkt::registry& registry_)
        : registry(&registry_)
    {
        rp3d::PhysicsWorld::WorldSettings settings;
        settings.gravity ;

        world = pc.createPhysicsWorld(settings);
        world->setEventListener(&listener);
    }

    ~physics_runtime()
    {
        // Clean up all rigid bodies before destroying the physics world.
        for (auto entity : registry->view<RigidBody3DComponent>()) {
            registry->get<RigidBody3DComponent>(entity).runtime.reset();
        }
        pc.destroyPhysicsWorld(world);
    }
};

struct rigid_body_runtime
{
    spkt::registry*     registry;
    spkt::entity         entity;
    rp3d::PhysicsWorld* world;
    rp3d::RigidBody*    body;

    rigid_body_runtime(
        spkt::registry* registry_,
        spkt::entity entity_,
        rp3d::PhysicsWorld* world_
    )
        : registry(registry_)
        , entity(entity_)
        , world(world_)
        , body(nullptr)
    {
        auto& tc = registry->get<Transform3DComponent>(entity);
        body = world->createRigidBody(convert(tc));
        body->setUserData(static_cast<void*>(&entity));
    }

    ~rigid_body_runtime()
    {
        // Reset any collider runtimes for this entity before deleting the
        // rigid body. This will invoke the collider runtime destructors to
        // clean them up in the physics world.
        if (auto c = registry->get_if<BoxCollider3DComponent>(entity)) {
            c->runtime.reset();
        }
        if (auto c = registry->get_if<SphereCollider3DComponent>(entity)) {
            c->runtime.reset();
        }
        if (auto c = registry->get_if<CapsuleCollider3DComponent>(entity)) {
            c->runtime.reset();
        }
        world->destroyRigidBody(body);
    }
};

struct collider_runtime
{
    rp3d::RigidBody* body;
    rp3d::Collider*  collider;

    collider_runtime(rp3d::RigidBody* _body, rp3d::Collider* _collider)
        : body(_body) , collider(_collider)
    {}

    ~collider_runtime()
    {
        body->removeCollider(collider);
    }
};

std::shared_ptr<collider_runtime> make_box_collider(
    rp3d::PhysicsCommon* pc,
    spkt::registry& registry,
    spkt::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& bc = registry.get<BoxCollider3DComponent>(entity);

    glm::vec3 dimensions = bc.halfExtents;
    if (bc.applyScale) { dimensions *= tc.scale; }
    rp3d::BoxShape* shape = pc->createBoxShape(convert(dimensions));
    rp3d::Transform transform = convert(bc.position, bc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    set_material(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

std::shared_ptr<collider_runtime> make_sphere_collider(
    rp3d::PhysicsCommon* pc,
    spkt::registry& registry,
    spkt::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& sc = registry.get<SphereCollider3DComponent>(entity);
    
    rp3d::SphereShape* shape = pc->createSphereShape(sc.radius);
    rp3d::Transform transform = convert(sc.position, sc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    set_material(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

std::shared_ptr<collider_runtime> make_capsule_collider(
    rp3d::PhysicsCommon* pc,
    spkt::registry& registry,
    spkt::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& cc = registry.get<CapsuleCollider3DComponent>(entity);
    
    rp3d::CapsuleShape* shape = pc->createCapsuleShape(cc.radius, cc.height);
    rp3d::Transform transform = convert(cc.position, cc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    set_material(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

namespace {

bool is_on_floor(spkt::registry& registry, rp3d::PhysicsWorld* const world, const rp3d::RigidBody* const body)
{
    // Get the point at the bottom of the rigid body.
    auto aabb = body->getAABB();
    rp3d::Vector3 playerBase = aabb.getCenter();
    playerBase.y = aabb.getMin().y;

    // Raycast down from this point. We actually raycast from slightly
    // higher which works more consistently for some reason. TODO: Find
    // out why
    rp3d::Vector3 up(0.0f, 1.0f, 0.0f);
    float delta = 0.1f;
    rp3d::Ray ray(playerBase + delta * up, playerBase - 2 * delta * up);
    raycast_callback cb;
    world->raycast(ray, &cb);
    return registry.valid(cb.entity);
}

PhysicsSingleton& get_physics_runtime(spkt::registry& registry)
{
    auto entity = registry.find<PhysicsSingleton>();
    if (!registry.valid(entity)) [[unlikely]] {
        entity = registry.create();
        registry.emplace<Runtime>(entity);
        registry.emplace<NameComponent>(entity, "::PhysicsRuntimeSingleton");
        auto& ps = registry.emplace<PhysicsSingleton>(entity);
        ps.physics_runtime = std::make_shared<physics_runtime>(registry);
    }
    return registry.get<PhysicsSingleton>(entity);
}

}

void physics_system(spkt::registry& registry, double dt)
{
    auto& ps = get_physics_runtime(registry);
    auto& runtime = *ps.physics_runtime;

    // Pre Update
    for (spkt::entity entity : registry.view<RigidBody3DComponent>()) {
        const auto& tc = registry.get<Transform3DComponent>(entity);
        auto& physics = registry.get<RigidBody3DComponent>(entity);

        if (!physics.runtime) [[unlikely]] {
            physics.runtime = std::make_shared<rigid_body_runtime>(
                &registry, entity, runtime.world
            );
        }

        rp3d::RigidBody* body = physics.runtime->body;

        if (registry.has<BoxCollider3DComponent>(entity)) {
            auto& cc = registry.get<BoxCollider3DComponent>(entity);
            if (!cc.runtime) [[unlikely]] {
                cc.runtime = make_box_collider(&runtime.pc, registry, entity, body);
            }
        }
        if (registry.has<SphereCollider3DComponent>(entity)) {
            auto& cc = registry.get<SphereCollider3DComponent>(entity);
            if (!cc.runtime) [[unlikely]] {
                cc.runtime = make_sphere_collider(&runtime.pc, registry, entity, body);
            }
        }
        if (registry.has<CapsuleCollider3DComponent>(entity)) {
            auto& cc = registry.get<CapsuleCollider3DComponent>(entity);
            if (!cc.runtime) [[unlikely]] {
                cc.runtime = make_capsule_collider(&runtime.pc, registry, entity, body);
            }
        }

        body->setTransform(convert(tc));
        body->setLinearVelocity(convert(physics.velocity));
        body->enableGravity(physics.gravity);    

        if (physics.frozen) {
            body->setType(rp3d::BodyType::STATIC);
        }
        else {
            body->setType(rp3d::BodyType::DYNAMIC);

            // TODO: Move to RigidBody3DComponent
            float mass = 0;
            if (registry.has<BoxCollider3DComponent>(entity)) { mass += registry.get<BoxCollider3DComponent>(entity).mass; }
            if (registry.has<SphereCollider3DComponent>(entity)) { mass += registry.get<SphereCollider3DComponent>(entity).mass; }
            if (registry.has<CapsuleCollider3DComponent>(entity)) { mass += registry.get<CapsuleCollider3DComponent>(entity).mass; }
            body->setMass(mass);
        }

        if (runtime.lastFrameLength > 0) {
            auto f = physics.force / runtime.lastFrameLength;
            body->applyForceToCenterOfMass(convert(f));
        }
    }
    
    // Update System
    runtime.lastFrameLength = 0;

    static float accumulator = 0.0f;
    accumulator += static_cast<float>(dt);

    // First update the Physics World.
    while (accumulator >= PHYSICS_TIME_STEP) {
        runtime.world->update(PHYSICS_TIME_STEP);
        accumulator -= PHYSICS_TIME_STEP;
        runtime.lastFrameLength += PHYSICS_TIME_STEP;
    }

    // Post Update
    for (auto entity : registry.view<RigidBody3DComponent>()) {
        auto& tc = registry.get<Transform3DComponent>(entity);
        auto& rc = registry.get<RigidBody3DComponent>(entity);
        const rp3d::RigidBody* body = rc.runtime->body;

        tc.position = convert(body->getTransform().getPosition());
        tc.orientation = convert(body->getTransform().getOrientation());
        rc.velocity = convert(body->getLinearVelocity());

        rc.force = {0.0, 0.0, 0.0};
        rc.onFloor = is_on_floor(registry, runtime.world, body);
    }

    // Publish all collision events
    for (const auto& [a, b] : runtime.listener.collisions()) {
        auto event = registry.create();
        registry.emplace<Runtime>(event);
        registry.emplace<Event>(event);
        registry.emplace<CollisionEvent>(event, a, b);
    }
    runtime.listener.collisions().clear();
}

}
