
#include <utility>
#include <memory>

#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include <box2d.h>
#include <types.h>

#include <Id.h>
#include <Engine.h>
#include <GameMode.h>
#include <GameModeTag.h>
#include <ShapeGraphicsComponent.h>
#include <GameObjectTag.h>
#include <TransformComponent.h>
#include <PhysicsComponent.h>
#include <Converter.h>
#include <iostream>

int main()
{
    ni::Engine engine("Shooter by Eric", sf::State::Fullscreen);

    auto my_mode = std::make_unique<ni::GameMode>();
    my_mode->RegisterTilemap("maps/level_01/map.json");

    ni::Id<GameObjectTag> ball = my_mode->CreateGameObject();

    ni::TransformComponent ball_transform;
    ball_transform.SetPositionInMeters({ 5, 5 });
    my_mode->GetComponentStore().AttachTransformComponent(ball, ball_transform);

    sf::CircleShape shape(30, 32);
    shape.setFillColor(sf::Color::Red);
    auto ball_graphics = std::make_unique<ni::ShapeGraphicsComponent<sf::CircleShape>>(shape);

    my_mode->GetComponentStore().AttachGraphicsComponent(ball, std::move(ball_graphics));

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = { 0.0f, 9.8f };
    my_mode->GetPhysicsEngine().CreateWorld(world_def);

    // BALL
    b2BodyDef ball_body_def = b2DefaultBodyDef();
    ball_body_def.type = b2_dynamicBody;
    ball_body_def.position = { 0.0f, 4.0f };
    ball_body_def.enableSleep = false;        

    b2Circle circle_shape = { {0, 0}, converter::PixelsToMeters(30.0f) };

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 1.0f;
    shape_def.material.friction = 0.3f;    
    
    b2WorldId world_id = my_mode->GetPhysicsEngine().GetWorldId();

    b2BodyId ball_body_id = b2CreateBody(world_id, &ball_body_def);
    //b2CreatePolygonShape(ball_body_id, &shape_def, &box_shape);
    b2CreateCircleShape(ball_body_id, &shape_def, &circle_shape);

    // GROUND
    //b2BodyDef ground_body_def = b2DefaultBodyDef();
    //ground_body_def.type = b2_staticBody;
    //ground_body_def.position = { 0.0f, 10.0f };
    //ground_body_def.enableSleep = false;

    //b2Polygon ground_polygon = b2MakeBox(10, 1);

    //b2ShapeDef ground_shape_def = b2DefaultShapeDef();
    //ground_shape_def.density = 1.0f;
    //ground_shape_def.material.friction = 0.3f;

    //b2BodyId ground_body_id = b2CreateBody(my_mode->GetPhysicsEngine().GetWorldId(), &ground_body_def);
    //b2CreatePolygonShape(ground_body_id, &ground_shape_def, &ground_polygon);

    ni::PhysicsComponent ball_physics(ball_body_id);
    my_mode->GetComponentStore().AttachPhysicsComponent(ball, ball_physics);
    
    ni::Id<GameModeTag> mode_id = engine.GetGameModeController().Register(std::move(my_mode));
    engine.GetGameModeController().SwitchTo(mode_id);

    engine.Run();
}