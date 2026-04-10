#include <NiEngine/PhysicsBodyComponent.h>

#include <box2d.h>

#include <NiEngine/TransformComponent.h>

ni::PhysicsBodyComponent::PhysicsBodyComponent(b2BodyId body_id) : body_id_(body_id)
{
}

void ni::PhysicsBodyComponent::PhysicsUpdate(TransformComponent& transform_component)
{
    b2Vec2 position = b2Body_GetPosition(body_id_);
    float  radians = b2Rot_GetAngle(b2Body_GetRotation(body_id_));

    transform_component.SetPositionInMeters(position);
    transform_component.SetRotation(radians);
}
