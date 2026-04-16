#pragma once

#include <collision.h>
#include <math_functions.h>
#include <id.h>
#include <box2d.h>

#include <cfloat>

#include "PhysicsComponent.h"
#include "TransformComponent.h"

namespace ni {

struct CastResult
{
    b2Vec2 point;
    b2Vec2 normal;
    b2BodyId bodyId;
    float fraction;
    bool hit = false;

    float ray_length;
    float pogo_rest_length;
};

static float CastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context)
{
    CastResult* result = (CastResult*)context;
    result->point = point;
    result->normal = normal;
    result->bodyId = b2Shape_GetBody(shapeId);
    result->fraction = fraction;
    result->hit = true;
    return fraction;
}

class PlatformerCharacterPhysicsComponent : public PhysicsComponent
{
public:
    PlatformerCharacterPhysicsComponent(b2Vec2 start_position, b2Capsule shape);

    void SolveMove(float throttle);
    void Jump();
    CastResult CastPogo(b2WorldId world_id);

private:
    void ApplyFriction();
    void Accelerate(b2Vec2 desired_velocity);

    static bool PlaneResultFcn(b2ShapeId shapeId, const b2PlaneResult* result, void* ctx)
    {
        PlatformerCharacterPhysicsComponent* self = static_cast<PlatformerCharacterPhysicsComponent*>(ctx);
        if (self->plane_count_< kMaxPlanes)
        {
            self->planes_[self->plane_count_++] = {
                result->plane,
                /*pushLimit=*/ FLT_MAX,
                /*push=*/      0.0f,
                /*clipVel=*/   true
            };
        }
        return true;
    }

    static constexpr int kMaxPlanes = 8;

    b2Transform      transform_;
    b2Vec2           velocity_;
    b2Capsule        capsule_;
    b2CollisionPlane planes_[kMaxPlanes] = {};

    bool  on_ground_     = false;
    bool  jump_released_ = true;

    float delta = 1 / 60.0f;
    float jump_speed_ = 10.0f;
    float max_speed_  = 6.0f;
    float accelerate_ = 20.0f;
    float friction_   = 8.0f;
    float gravity_    = 9.8f;
    float air_steer_  = 0.2f;
    float min_speed_  = 0.1f;
    
    // Pogo attributes
    float pogo_velocity_ = 0.0f;
    float pogo_hertz_ = 5.0f;
    float pogo_damping_ratio_ = 0.8f;

    // Collision
    int total_iterations_ = 0;
    int plane_count_ = 0;

    // Inherited via PhysicsComponent
    void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id) override;
};

}