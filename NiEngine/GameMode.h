#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"
#include "ComponentStore.h"
#include "GameObjectId.h"

namespace ni {

class GameMode
{
private:
	ComponentStore store_;

	GameObjectId current_game_object_id_;

public:
	ComponentStore& getComponentStore()
	{
		return store_;
	}

	GameObjectId getNextGameObjectId() 
	{
		return ++current_game_object_id_;
	}

	virtual void update();
	virtual void render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}