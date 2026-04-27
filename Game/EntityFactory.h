#pragma once

#include <SFML/System/Vector2.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class EntityFactory
{
public:
	ni::Id<ni::GameObjectTag> CreatePlatformerCharacter(ni::GameMode& game_mode, sf::Vector2i character_size, int animation_row);
	ni::Id<ni::GameObjectTag> CreateObstacle(ni::GameMode& game_mode, sf::Vector2i obstacle_size, sf::Vector2i texture_coords, bool collidable = true, bool harmful = false);	
};

