#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace ni {

struct TilemapUtility 
{
	static sf::IntRect GetTextureBounds(sf::Vector2i coordinates, int tile_size = 0, int spacing = 0)
	{
		sf::IntRect result;
		result.size = { tile_size, tile_size };
		result.position.x = (coordinates.x + spacing) * tile_size;
		result.position.y = (coordinates.y + spacing) * tile_size;

		return result;
	}
};

}
