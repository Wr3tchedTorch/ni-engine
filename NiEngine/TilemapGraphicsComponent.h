#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "TilesetBlueprint.h"
#include "BitmapStore.h"

namespace ni {

struct TilesetDrawable
{
	sf::VertexArray vertices_ = {};
	sf::Vector2f	position_offset_ = {};
};

class TilemapGraphicsComponent
{
private:
	std::unordered_map<std::string, TilesetDrawable> tileset_drawables_;

	const TilesetBlueprint& GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid);

public:
	void AddTile(const sf::Vector2i& grid_position, int tile_id, const std::vector<TilesetBlueprint>& tileset_blueprints, const sf::Vector2f& layer_position_offset);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);

	sf::FloatRect GetBounds() const;
};

}