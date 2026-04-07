#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <string>

#include "BitmapStore.h"
#include "ComponentStore.h"
#include "Id.h"
#include "GameObjectTag.h"
#include "Tilemap.h"
#include "PhysicsEngine.h"

namespace ni {

class GameModeController;

class GameMode
{
private:
	ComponentStore component_store_;
	PhysicsEngine  physics_engine_;

	Id<GameObjectTag> current_game_object_id_;	

	std::vector<Tilemap> tilemaps_;

public:
	GameMode() = default;

	ComponentStore& GetComponentStore()
	{
		return component_store_;
	}

	PhysicsEngine& GetPhysicsEngine()
	{
		return physics_engine_;
	}

	Id<GameObjectTag> CreateGameObject()
	{
		return ++current_game_object_id_;
	}

	void RegisterTilemap(const std::string& filepath, bool enable_collision = true);

	virtual void PhysicsUpdate();
	virtual void Update(GameModeController& controller);
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}