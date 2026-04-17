#include <NiEngine/PlatformerCharacterPhysicsComponent.h>

#include <collision.h>
#include <math_functions.h>
#include <box2d.h>
#include <types.h>

#include <iostream>
#include <format>

#include <NiEngine/TransformComponent.h>
#include <NiEngine/CollisionBits.h>


ni::PlatformerCharacterPhysicsComponent::PlatformerCharacterPhysicsComponent(b2Vec2 start_position, b2Capsule shape)
{
    transform_ = { start_position, b2Rot_identity };
    velocity_  = { 0.0f, 0.0f };

    capsule_ = shape;
}

void ni::PlatformerCharacterPhysicsComponent::SolveMove(float throttle)
{    
    float speed = b2Length(velocity_);
    if (speed < min_speed_)
    {
        velocity_.x = 0.0f;
        velocity_.y = 0.0f;
    }
    else if (on_ground_)
    {
        ApplyFriction();
    }    
    b2Vec2 desired_velocity = { max_speed_ * throttle, 0.0f };
    Accelerate(desired_velocity);
}

void ni::PlatformerCharacterPhysicsComponent::Move(int throttle)
{
    throttle_ = throttle;
}

void ni::PlatformerCharacterPhysicsComponent::Jump()
{
    if (on_ground_)
    {
        velocity_.y = -jump_speed_;
        on_ground_  = false;
    }
}

ni::CastResult ni::PlatformerCharacterPhysicsComponent::CastPogo(b2WorldId world_id)
{
    float pogo_rest_length = 3.0f * capsule_.radius;
    float ray_length = pogo_rest_length + capsule_.radius;

    b2Vec2 origin = b2TransformPoint(transform_, capsule_.center2);

    b2Vec2 segmentOffset = { 0.75f * capsule_.radius, 0.0f };
    b2Segment segment = {
        .point1 = origin - segmentOffset,
        .point2 = origin + segmentOffset,
    };

    b2ShapeProxy proxy = {};
    b2Vec2 translation;
    b2QueryFilter pogo_filter = { MoverBit, StaticBit | DynamicBit };
    CastResult result = {};

    proxy = b2MakeProxy(&segment.point1, 2, 0.0f);
    translation = { 0.0f, ray_length };

    b2World_CastShape(world_id, &proxy, translation, pogo_filter, CastCallback, &result);

    result.pogo_rest_length = pogo_rest_length;
    result.ray_length = ray_length;

    return result;
}

void ni::PlatformerCharacterPhysicsComponent::ApplyFriction()
{
    float speed = b2Length(velocity_);

    float control = speed < stop_speed_ ? stop_speed_ : speed;

    float drop = control * friction_ * delta;
    float new_speed = b2MaxFloat(0.0f, speed - drop);
    velocity_ *= new_speed / speed;
}

void ni::PlatformerCharacterPhysicsComponent::Accelerate(b2Vec2 desired_velocity)
{
    float  desired_speed = 0.0f;
    b2Vec2 desired_direction = b2GetLengthAndNormalize(&desired_speed, desired_velocity);

    if (desired_speed > max_speed_)
    {
        desired_speed = max_speed_;
    }
    if (on_ground_)
    {
        velocity_.y = 0.0f;
    }

    float current_speed = b2Dot(velocity_, desired_direction);
    float add_speed = desired_speed - current_speed;

    if (add_speed > 0.0f)
    {
        float steer = on_ground_ ? 1.0f : air_steer_;

        float accel_speed = steer * accelerate_ * max_speed_ * delta;
        if (accel_speed > add_speed)
        {
            accel_speed = add_speed;
        }

        velocity_ += accel_speed * desired_direction;
    }
    
    velocity_.y += gravity_ * delta;
}

void ni::PlatformerCharacterPhysicsComponent::PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id)
{
    SolveMove(throttle_);

    // POGO COLLISION
    CastResult cast_result = CastPogo(world_id);

    if (on_ground_ == false)
    {
        on_ground_ = cast_result.hit && velocity_.y >= -0.01f;
    }
    else
    {
        on_ground_ = cast_result.hit;
    }

    if (cast_result.hit)
    {        
        float pogo_current_length = cast_result.fraction * cast_result.ray_length;

        float zeta    = pogo_damping_ratio_;
        float hertz   = pogo_hertz_;
        float omega   = 2.0f * B2_PI * hertz;
        float omega_h = omega * delta;

        pogo_velocity_ = (pogo_velocity_ - omega * omega_h * (pogo_current_length - cast_result.pogo_rest_length)) /
            (1.0f + 2.0f * zeta * omega_h + omega_h * omega_h);

        b2Body_ApplyForce(cast_result.bodyId, { 0.0f, 50.0f }, cast_result.point, true);
    }
    else
    {
        pogo_velocity_ = 0.0f;
    }
    
    // PLANE COLLISION
    b2Vec2 target = transform_.p + delta * velocity_ + delta * pogo_velocity_ * b2Vec2{ 0.0f, -1.0f };

    // Mover overlap filter
    b2QueryFilter collide_filter = { MoverBit, StaticBit | DynamicBit | MoverBit };

    // Movers don't sweep against other movers, allows for soft collision
    b2QueryFilter cast_filter = { MoverBit, StaticBit | DynamicBit };

    total_iterations_ = 0;
    float tolerance   = 0.01f;

    for (int iteration = 0; iteration < 5; ++iteration)
    {
        plane_count_ = 0;

        b2Capsule mover{};
        mover.center1 = b2TransformPoint(transform_, capsule_.center1);
        mover.center2 = b2TransformPoint(transform_, capsule_.center2);
        mover.radius = capsule_.radius;

        b2World_CollideMover(world_id, &mover, collide_filter, PlaneResultFcn, this);
        b2PlaneSolverResult result = b2SolvePlanes(target, planes_, plane_count_);

        total_iterations_ += result.iterationCount;

        b2Vec2 mover_translation = result.position - transform_.p;

        float fraction = b2World_CastMover(world_id, &mover, mover_translation, cast_filter);

        b2Vec2 time_step = fraction * mover_translation;
        transform_.p += time_step;

        if (b2LengthSquared(time_step) < tolerance * tolerance)
        {
            break;
        }
    }

    velocity_ = b2ClipVector(velocity_, planes_, plane_count_);

    transform_component.SetPositionInMeters(transform_.p);
    transform_component.SetRotation(b2Rot_GetAngle(transform_.q));
}
