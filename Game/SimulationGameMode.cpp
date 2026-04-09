#include "SimulationGameMode.h"

#include <format>
#include <string>

#include <types.h>
#include <math_functions.h>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/Converter.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/BitmapStore.h>

SimulationGameMode::SimulationGameMode()
{
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = { 0.0f, 9.8f };

    GetPhysicsEngine().CreateWorld(world_def);

    std::string level_to_load = "level_02";

    ni::Converter::PIXELS_PER_METERS = 32.0f;
    RegisterTilemap(std::format("maps/{}/{}.json", level_to_load, level_to_load));

    main_camera_.FitTo(tilemaps_.front().GetBounds());
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
