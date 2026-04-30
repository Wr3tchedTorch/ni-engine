#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <string>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/ComponentStore.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Level.h>
#include <NiEngine/PhysicsEngine.h>

namespace ni {

class GameModeController;

class GameMode
{
private:
	Id<GameObjectTag> current_game_object_id_{ 0 };

	bool box2d_enabled = false;

protected:
	Level level_;

	ComponentStore component_store_;
	PhysicsEngine  physics_engine_;

public:
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

	void LoadLevel(int index, bool enable_tilemap_collisions = true);

	void SetBox2dEnabled(bool value)
	{
		box2d_enabled = value;
	}

	virtual void PhysicsUpdate(float delta);
	virtual void Update(GameModeController& controller);
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}