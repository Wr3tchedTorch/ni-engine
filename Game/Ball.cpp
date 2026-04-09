#include "Ball.h"

#include <box2d.h>
#include <types.h>
#include <collision.h>
#include <math_functions.h>

#include <memory>
#include <utility>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Converter.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/ShapeGraphicsComponent.h>
#include <NiEngine/ComponentStore.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/Id.h>

Ball::Ball(ni::Id<GameObjectTag> id, b2Vec2 starting_position, ni::ComponentStore& component_store, b2WorldId world_id, sf::Color color, float size)
{
    id_ = id;

    // GRAPHICS
    ni::TransformComponent ball_transform;
    ball_transform.SetPositionInMeters(starting_position);

    component_store.AttachTransformComponent(id, ball_transform);

    sf::RectangleShape shape({ni::Converter::MetersToPixels(size), ni::Converter::MetersToPixels(size)});
    shape.setFillColor(color);
    shape.setOrigin({ ni::Converter::MetersToPixels(size) / 2.f, ni::Converter::MetersToPixels(size) / 2.f });    
    
    auto ball_graphics = std::make_unique<ni::ShapeGraphicsComponent<sf::RectangleShape>>(shape);

    component_store.AttachGraphicsComponent(id, std::move(ball_graphics));

    // PHYSICS
    b2BodyDef ball_body_def = b2DefaultBodyDef();
    ball_body_def.type        = b2_dynamicBody;
    ball_body_def.position    = starting_position;
    ball_body_def.enableSleep = false;
    ball_body_def.isBullet = true;

    b2Polygon box_shape = b2MakeBox(size / 2.f, size / 2.f);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 1.0f;
    shape_def.material.friction = 0.3f;

    b2BodyId ball_body_id = b2CreateBody(world_id, &ball_body_def);
    b2CreatePolygonShape(ball_body_id, &shape_def, &box_shape);

    ni::PhysicsComponent ball_physics(ball_body_id);
    component_store.AttachPhysicsComponent(id, ball_physics);
}
