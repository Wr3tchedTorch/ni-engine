#include <NiEngine/GameMode.h>

#include <string>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/Tilemap.h>

void ni::GameMode::RegisterTilemap(const std::string& filepath, bool enable_collision)
{	
	Tilemap map;

	if (enable_collision)
	{
		map.EnableCollision(physics_engine_.GetWorldId());
	}

	map.LoadFromFile(filepath);

	tilemaps_.push_back(std::move(map));
}

void ni::GameMode::PhysicsUpdate()
{
	if (box2d_enabled)
	{
		physics_engine_.PhysicsUpdate();
	}

	auto* tilemap = tilemaps_.empty() ? nullptr : &tilemaps_.front();
	component_store_.PhysicsUpdate(physics_engine_.GetWorldId(), tilemap);
}

void ni::GameMode::Update(GameModeController& controller)
{	
	component_store_.Update();
}

void ni::GameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	for (auto& map : tilemaps_)
	{
		map.Render(target, states, store);
	}
	component_store_.Render(target, states, store);
}
