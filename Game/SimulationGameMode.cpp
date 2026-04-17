#include "SimulationGameMode.h"

#include <types.h>
#include <math_functions.h>
#include <collision.h>

#include <memory>
#include <utility>
#include <format>
#include <string>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/Converter.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/PlatformerCharacterPhysicsComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ShapeGraphicsComponent.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "PlayerUpdateComponent.h"
#include <SFML/Graphics/Rect.hpp>

SimulationGameMode::SimulationGameMode()
{
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = { 0.0f, 9.8f };

    GetPhysicsEngine().CreateWorld(world_def);

    std::string level_to_load = "level_01";

    ni::Converter::PIXELS_PER_METERS = 16.0f;
    RegisterTilemap(std::format("maps/{}/{}.json", level_to_load, level_to_load));

    main_camera_.FitTo(tilemaps_.front().GetBounds());

    SpawnPlayer();
}

void SimulationGameMode::Update(ni::GameModeController& controller)
{
    GameMode::Update(controller);

    if (IsMouseButtonJustPressed(sf::Mouse::Button::Left))
    {
        ball_factory_.SpawnRandomizedBall(*this, ni::Converter::PixelsToMeters(current_mouse_position_), { 0, 0 });
    }

    if (IsMouseButtonJustPressed(sf::Mouse::Button::Right))
    {
        ball_factory_.SpawnRandomizedBall(*this, ni::Converter::PixelsToMeters(current_mouse_position_), { 0, -20 });
    }
}

void SimulationGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
    main_camera_.ApplyTo(target);

    GameMode::Render(target, states, store);

    current_mouse_position_ = main_camera_.GetCoordinatesFromPixel(target, sf::Mouse::getPosition());
}

bool SimulationGameMode::IsMouseButtonJustPressed(sf::Mouse::Button button)
{
    bool is_pressed = sf::Mouse::isButtonPressed(button);

    bool is_just_pressed = is_pressed && !mouse_pressed_history_[button];

    mouse_pressed_history_[button] = is_pressed;

    return is_just_pressed;
}

void SimulationGameMode::SpawnPlayer()
{
    ni::Id<GameObjectTag> player = CreateGameObject();

    ni::TransformComponent transform_comp;

    auto physics_comp = std::make_unique<ni::PlatformerCharacterPhysicsComponent>(b2Vec2({ 5, 5 }), b2Capsule({ { 0.0f, -0.4f }, { 0.0f, 0.4f }, 0.3f }));
    auto update_comp  = std::make_unique<PlayerUpdateComponent>(player, GetComponentStore());

    float radius_px = ni::Converter::MetersToPixels(0.3f);
    auto shapes = GetPlayerShape(radius_px);

    auto graphics_comp  = std::make_unique<ni::ShapeGraphicsComponent<sf::CircleShape>>(shapes.first);
    auto graphics_comp2 = std::make_unique<ni::ShapeGraphicsComponent<sf::CircleShape>>(shapes.second);

    auto graphics_comp3 = std::make_unique<ni::AnimatedGraphicsComponent>("graphics/tilemap.png", sf::IntRect({{17, 204}, {16, 16}}), 2, 1, 1);
    
    GetComponentStore().AttachUpdateComponent(player, std::move(update_comp));
    GetComponentStore().AttachTransformComponent(player, transform_comp);
    GetComponentStore().AttachPhysicsComponent(player, std::move(physics_comp));
    //GetComponentStore().AttachGraphicsComponent(player, std::move(graphics_comp));
    //GetComponentStore().AttachGraphicsComponent(player, std::move(graphics_comp2));
    GetComponentStore().AttachGraphicsComponent(player, std::move(graphics_comp3));
}

std::pair<sf::CircleShape, sf::CircleShape> SimulationGameMode::GetPlayerShape(float radius)
{
    std::pair<sf::CircleShape, sf::CircleShape> result;

    sf::CircleShape player_shape(ni::Converter::MetersToPixels(0.3f));
    player_shape.setFillColor(sf::Color::White);

    player_shape.setOrigin({ 
        radius, 
        radius + ni::Converter::MetersToPixels(0.4f)
    });

    result.first = player_shape;

    player_shape.setOrigin({
        radius,
        radius - ni::Converter::MetersToPixels(0.4f)
    });

    result.second = player_shape;

    return result;
}
