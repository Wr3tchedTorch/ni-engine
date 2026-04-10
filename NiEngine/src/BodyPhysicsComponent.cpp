#include <NiEngine/BodyPhysicsComponent.h>

#include <math_functions.h>
#include <box2d.h>
#include <id.h>

#include <NiEngine/TransformComponent.h>

ni::BodyPhysicsComponent::BodyPhysicsComponent(b2BodyId body_id) : body_id_(body_id)
{
}

void ni::BodyPhysicsComponent::PhysicsUpdate(TransformComponent& transform_component)
{
    b2Vec2 position = b2Body_GetPosition(body_id_);
    float  radians = b2Rot_GetAngle(b2Body_GetRotation(body_id_));

    transform_component.SetPositionInMeters(position);
    transform_component.SetRotation(radians);
}
