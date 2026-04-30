#include <NiEngine/GameMode.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameModeController.h>

void ni::GameMode::LoadLevel(int index, bool enable_tilemap_collisions)
{
	level_.LoadLevel(index);
	if (enable_tilemap_collisions)
	{
		level_.EnableTilemapCollisions(physics_engine_.GetWorldId());
	}
}

void ni::GameMode::PhysicsUpdate(float delta)
{
	if (box2d_enabled)
	{
		physics_engine_.PhysicsUpdate();
	}

	component_store_.PhysicsUpdate(physics_engine_.GetWorldId(), level_.GetCurrentTilemap(), delta);
}

void ni::GameMode::Update(GameModeController& controller)
{
	component_store_.Update();
}

void ni::GameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	level_.RenderTilemap(target, states, store);
	component_store_.Render(target, states, store);
}
