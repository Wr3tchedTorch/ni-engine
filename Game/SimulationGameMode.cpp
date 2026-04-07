#include "SimulationGameMode.h"

#include <types.h>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <GameMode.h>
#include <Converter.h>
#include <GameModeController.h>
#include <PhysicsComponent.h>
#include <box2d.h>

#include "Ball.h"

SimulationGameMode::SimulationGameMode()
{
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = { 0.0f, 9.8f };

    GetPhysicsEngine().CreateWorld(world_def);

    RegisterTilemap("maps/level_01/map.json");
}

void SimulationGameMode::Update(ni::GameModeController& controller)
{
    GameMode::Update(controller);

    if (IsMouseButtonJustPressed(sf::Mouse::Button::Left))
    {
        ball_factory_.SpawnRandomizedBall(*this, converter::PixelsToMeters(sf::Mouse::getPosition()), { 0, 0 });
    }

    if (IsMouseButtonJustPressed(sf::Mouse::Button::Right))
    {
        ball_factory_.SpawnRandomizedBall(*this, converter::PixelsToMeters(sf::Mouse::getPosition()), { 0, -10 });
    }
}

bool SimulationGameMode::IsMouseButtonJustPressed(sf::Mouse::Button button)
{
    bool is_pressed = sf::Mouse::isButtonPressed(button);

    bool is_just_pressed = is_pressed && !mouse_pressed_history_[button];

    mouse_pressed_history_[button] = is_pressed;

    return is_just_pressed;
}
