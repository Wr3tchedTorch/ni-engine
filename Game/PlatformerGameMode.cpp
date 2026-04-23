#include "PlatformerGameMode.h"

#include <types.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>

PlatformerGameMode::PlatformerGameMode()
{
	ni::Converter::pixels_per_meters_ = 16;

	b2WorldDef world_def = b2DefaultWorldDef();
	world_def.gravity = { 0.0f, 9.8f };
	GetPhysicsEngine().CreateWorld(world_def);

	RegisterTilemap("maps/level_01/level_01.json");

	camera_.FitTo(tilemaps_.front().GetBounds());

	entity_factory_.CreatePlatformerCharacter(*this, { 16, 16 }, 12);
}

void PlatformerGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	camera_.ApplyTo(target);

	ni::GameMode::Render(target, states, store);
}
