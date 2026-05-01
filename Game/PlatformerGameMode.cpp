#include "PlatformerGameMode.h"

#include <types.h>
#include <memory>
#include <utility>
#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>

#include "PlatformerObjectFactory.h"

PlatformerGameMode::PlatformerGameMode()
{
	ni::Converter::pixels_per_meters_ = 16;

	b2WorldDef world_def = b2DefaultWorldDef();
	world_def.gravity = { 0.0f, 9.8f };
	GetPhysicsEngine().CreateWorld(world_def);
	
	auto factory = std::make_unique<PlatformerObjectFactory>();
	level_.RegisterObjectFactory(std::move(factory));
	level_.LoadNextLevel(*this);

	camera_.FitTo(level_.GetCurrentTilemap().GetBounds());
}

void PlatformerGameMode::PrepareToLoadNextLevel()
{
	load_next_level_ = true;
}

void PlatformerGameMode::RestartLevel()
{
	restart_level_ = true;
}

void PlatformerGameMode::Update(ni::GameModeController& controller)
{
	if (restart_level_)
	{
		level_.ReloadLevel(*this);
		restart_level_ = false;
		return;
	}
	if (load_next_level_)
	{
		LoadNextLevel(false);
		load_next_level_ = false;
		return;
	}
	GameMode::Update(controller);
}

void PlatformerGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	camera_.ApplyTo(target);

	ni::GameMode::Render(target, states, store);
}
