#pragma once

#include <id.h>
#include <math_functions.h>

#include <vector>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>

#include "LayerBlueprint.h"
#include "Vector2iHash.h"
#include "TilesetBlueprint.h"

namespace ni {

struct LoopInformation
{
	bool is_closed_ = false;
	std::vector<b2Vec2> data_;
};

using EdgesMap = std::unordered_map<sf::Vector2i, sf::Vector2i, Vector2iHash>;

class TilemapCollisionComponent
{
private:
	b2BodyId body_id_ = {};

	EdgesMap exposed_edges_   = {};
	EdgesMap one_sided_edges_ = {};

	bool IsTileEmpty(const std::vector<int>& map, sf::Vector2i map_size, sf::Vector2i tile_grid_position);
	bool IsTileEmpty(const std::vector<int>& map, int tile_index);

	std::vector<LoopInformation> GetCollisionLoops(EdgesMap& map, sf::Vector2i tile_size);

	void CreateOnesidedCollision(sf::Vector2i tile_size);

public:
	TilemapCollisionComponent(b2WorldId world_id);

	void AddTile(sf::Vector2i grid_position, int tile_gid, const TilesetBlueprint& tileset, const LayerBlueprint& layer, sf::Vector2i map_size);
	void CreateCollision(sf::Vector2i tile_size);
};

}