#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "TilesetBlueprint.h"
#include "BitmapStore.h"

namespace ni {

class TilemapGraphicsComponent
{

private:
	std::unordered_map<std::string, sf::VertexArray> vertices_by_tileset_;

	const TilesetBlueprint& GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid);

public:
	void AddTile(const sf::Vector2i& grid_position, int tile_id, const std::vector<TilesetBlueprint>& tileset_blueprints);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}