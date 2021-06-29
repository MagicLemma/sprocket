#include "PhysicsEngine3D.h"
#include "ECS.h"
#include "Log.h"
#include "Scene.h"
#include "Components.h"

#include <variant>
#include <unordered_map>

#include <reactphysics3d/reactphysics3d.h>

namespace spkt {
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
    std::vector<std::pair<apx::entity, apx::entity>> d_collisions;

public:
    void onContact(const rp3d::CollisionCallback::CallbackData& data) override
    {
        for (u32 p = 0; p != data.getNbContactPairs(); ++p) {
            auto pair = data.getContactPair(p);
            auto type = pair.getEventType();
            if (type == rp3d::CollisionCallback::ContactPair::EventType::ContactStart) {
                spkt::entity e1 = *static_cast<spkt::entity*>(pair.getBody1()->getUserData());
                spkt::entity e2 = *static_cast<spkt::entity*>(pair.getBody2()->getUserData());
                d_collisions.push_back(std::make_pair(e1.entity(), e2.entity()));
            }
        }
    }

    std::vector<std::pair<apx::entity, apx::entity>>& collisions() { return d_collisions; }
};

}

struct physics_runtime
{
    spkt::registry* registry;

    rp3d::PhysicsCommon pc;
    rp3d::PhysicsWorld* world;

    EventListener listener;

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
    rp3d::PhysicsWorld* world;
    rp3d::RigidBody*    body;
    spkt::entity        handle;

    rigid_body_runtime(
        spkt::registry& registry_,
        apx::entity entity_,
        rp3d::PhysicsWorld* world_
    )
        : world(world_)
        , body(nullptr)
        , handle({registry_, entity_})
    {
        auto& tc = handle.get<Transform3DComponent>();
        body = world->createRigidBody(Convert(tc));
        body->setUserData(static_cast<void*>(&handle));
    }

    ~rigid_body_runtime()
    {
        // Reset any collider runtimes for this entity before deleting the
        // rigid body. This will invoke the collider runtime destructors to
        // clean them up in the physics world.
        if (auto c = handle.get_if<BoxCollider3DComponent>()) {
            c->runtime.reset();
        }
        if (auto c = handle.get_if<SphereCollider3DComponent>()) {
            c->runtime.reset();
        }
        if (auto c = handle.get_if<CapsuleCollider3DComponent>()) {
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
    apx::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& bc = registry.get<BoxCollider3DComponent>(entity);

    glm::vec3 dimensions = bc.halfExtents;
    if (bc.applyScale) { dimensions *= tc.scale; }
    rp3d::BoxShape* shape = pc->createBoxShape(Convert(dimensions));
    rp3d::Transform transform = Convert(bc.position, bc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    SetMaterial(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

std::shared_ptr<collider_runtime> make_sphere_collider(
    rp3d::PhysicsCommon* pc,
    spkt::registry& registry,
    apx::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& sc = registry.get<SphereCollider3DComponent>(entity);
    
    rp3d::SphereShape* shape = pc->createSphereShape(sc.radius);
    rp3d::Transform transform = Convert(sc.position, sc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    SetMaterial(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

std::shared_ptr<collider_runtime> make_capsule_collider(
    rp3d::PhysicsCommon* pc,
    spkt::registry& registry,
    apx::entity entity,
    rp3d::RigidBody* rigid_body
) {
    auto& tc = registry.get<Transform3DComponent>(entity);
    auto& cc = registry.get<CapsuleCollider3DComponent>(entity);
    
    rp3d::CapsuleShape* shape = pc->createCapsuleShape(cc.radius, cc.height);
    rp3d::Transform transform = Convert(cc.position, cc.orientation);

    rp3d::Collider* collider = rigid_body->addCollider(shape, transform);
    SetMaterial(collider, registry.get<RigidBody3DComponent>(entity));

    return std::make_shared<collider_runtime>(rigid_body, collider);
}

namespace {

bool is_on_floor(const rp3d::PhysicsWorld* const world, const rp3d::RigidBody* const body)
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
    RaycastCB cb;
    world->raycast(ray, &cb);
    return cb.GetEntity().valid();
}

}

void PhysicsEngine3D::on_startup(spkt::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    registry.emplace<CameraSingleton>(singleton);
    auto& ps = registry.emplace<PhysicsSingleton>(singleton);
    ps.physics_runtime = std::make_shared<physics_runtime>(registry);
}

void PhysicsEngine3D::on_update(spkt::registry& registry, double dt)
{
    auto singleton = registry.find<Singleton>();
    if (!registry.valid(singleton)) {
        return;
    }

    auto& ps = get_singleton<PhysicsSingleton>(registry);
    auto& runtime = *ps.physics_runtime;

    // Pre Update
    for (auto id : registry.view<RigidBody3DComponent>()) {
        spkt::entity entity{registry, id};
        const auto& tc = entity.get<Transform3DComponent>();
        auto& physics = entity.get<RigidBody3DComponent>();

        if (!physics.runtime) {
            physics.runtime = std::make_shared<rigid_body_runtime>(
                registry, id, runtime.world
            );
        }

        rp3d::RigidBody* body = physics.runtime->body;

        if (entity.has<BoxCollider3DComponent>()) {
            auto& cc = entity.get<BoxCollider3DComponent>();
            if (!cc.runtime) {
                cc.runtime = make_box_collider(&runtime.pc, registry, id, body);
            }
        }
        if (entity.has<SphereCollider3DComponent>()) {
            auto& cc = entity.get<SphereCollider3DComponent>();
            if (!cc.runtime) {
                cc.runtime = make_sphere_collider(&runtime.pc, registry, id, body);
            }
        }
        if (entity.has<CapsuleCollider3DComponent>()) {
            auto& cc = entity.get<CapsuleCollider3DComponent>();
            if (!cc.runtime) {
                cc.runtime = make_capsule_collider(&runtime.pc, registry, id, body);
            }
        }

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

        if (runtime.lastFrameLength > 0) {
            auto f = physics.force / runtime.lastFrameLength;
            body->applyForceToCenterOfMass(Convert(f));
        }
    }
    
    // Update System
    runtime.lastFrameLength = 0;

    static float accumulator = 0.0f;
    accumulator += static_cast<float>(dt);

    // First update the Physics World.
    while (accumulator >= TIME_STEP) {
        runtime.world->update(TIME_STEP);
        accumulator -= TIME_STEP;
        runtime.lastFrameLength += TIME_STEP;
    }

    // Post Update
    for (auto entity : registry.view<RigidBody3DComponent>()) {
        auto& tc = registry.get<Transform3DComponent>(entity);
        auto& rc = registry.get<RigidBody3DComponent>(entity);
        const rp3d::RigidBody* body = rc.runtime->body;

        tc.position = Convert(body->getTransform().getPosition());
        tc.orientation = Convert(body->getTransform().getOrientation());
        rc.velocity = Convert(body->getLinearVelocity());

        rc.force = {0.0, 0.0, 0.0};
        rc.onFloor = is_on_floor(runtime.world, body);
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

#if 0
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
#endif

}
