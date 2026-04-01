#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"
#include "ComponentStore.h"
#include "Id.h"
#include "GameObjectTag.h"

namespace ni {

class GameMode
{
private:
	ComponentStore store_;

	Id<GameObjectTag> current_game_object_id_;

public:
	GameMode() = default;

	ComponentStore& GetComponentStore()
	{
		return store_;
	}

	Id<GameObjectTag> CreateGameObject()
	{
		return ++current_game_object_id_;
	}

	virtual void Update();
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}