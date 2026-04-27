#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class EntityFactory
{
public:
	ni::Id<ni::GameObjectTag> CreatePlatformerCharacter(ni::GameMode& game_mode, sf::Vector2i character_size, int animation_row);
	ni::Id<ni::GameObjectTag> CreateObstacle(ni::GameMode& game_mode, ni::Id<ni::GameObjectTag> player_id, sf::Vector2i position, sf::IntRect texture_rect, bool collidable = true, bool harmful = false);
};

