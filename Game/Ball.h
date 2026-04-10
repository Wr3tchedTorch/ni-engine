#pragma once

#include <math_functions.h>
#include <id.h>

#include <SFML/Graphics/Color.hpp>
#include <NiEngine/ComponentStore.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class Ball
{
private:
	ni::Id<GameObjectTag> id_;	

public:
	Ball(ni::Id<GameObjectTag> id, b2Vec2 starting_position, ni::ComponentStore& component_store, b2WorldId world_id, sf::Color color = sf::Color::Red, float size = 1);

	ni::Id<GameObjectTag> GetId() const
	{
		return id_;
	}
};

