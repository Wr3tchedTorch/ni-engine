#include "PlatformerGameMode.h"

#include <types.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/TilemapUtility.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

PlatformerGameMode::PlatformerGameMode()
{
	ni::Converter::pixels_per_meters_ = 16;

	b2WorldDef world_def = b2DefaultWorldDef();
	world_def.gravity = { 0.0f, 9.8f };
	GetPhysicsEngine().CreateWorld(world_def);

	RegisterTilemap("maps/level_00/level_00.json");

	camera_.FitTo(tilemaps_.front().GetBounds());

	ni::Id<ni::GameObjectTag> player_id = entity_factory_.CreatePlatformerCharacter(*this, { 16, 16 }, 12);

	entity_factory_.CreateObstacle(*this, player_id, {14*16, 16*16}, ni::TilemapUtility::GetTextureBounds({ 3, 9 }, 16, 1), true, true);
}

void PlatformerGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	camera_.ApplyTo(target);

	ni::GameMode::Render(target, states, store);
}
