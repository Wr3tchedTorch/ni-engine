#pragma once

#include <id.h>

#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "TilemapBlueprint.h"
#include "TilemapGraphicsComponent.h"
#include "TilesetBlueprint.h"
#include "TilesetReference.h"
#include "LayerBlueprint.h"
#include "BitmapStore.h"
#include "TilemapCollisionComponent.h"
#include "TileBlueprint.h"

namespace ni {

class Tilemap
{
private:
	TilemapBlueprint blueprint_;

	std::vector<TilesetBlueprint> tileset_blueprints_ = {};

	TilemapGraphicsComponent   graphics_;
	std::unique_ptr<TilemapCollisionComponent> collision_ = nullptr;

	std::string last_loaded_file_ = "";

	bool has_loaded_collision = false;
	bool has_loaded_graphics  = false;

	bool is_collision_enabled = false;

	void LoadTilesetBlueprints(const std::vector<TilesetReference>& tileset_references);
	void LoadTiles(const LayerBlueprint& layer);

public:
	inline static const std::string kPrototypeLayerName = "prototype";
	inline static const std::string kTransparentTilesTilesetName = "transparent_tiles";

	void EnableCollision(b2WorldId world_id);
	bool LoadFromFile(const std::string& filepath);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);

	sf::FloatRect GetBounds() const;

	sf::Vector2i GlobalToGridPosition(sf::Vector2f position) const;

	TileBlueprint GetTileInfo(sf::Vector2i tile_grid_position, int layer_index) const;
	bool IsTileEmpty(sf::Vector2i tile_grid_position) const;
	static bool IsTileEmpty(const std::vector<int>& map, sf::Vector2i map_size, sf::Vector2i tile_grid_position);
	static const TilesetBlueprint& GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid);
};

}