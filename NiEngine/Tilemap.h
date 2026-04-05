#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "TilemapBlueprint.h"
#include "TilemapGraphicsComponent.h"
#include "TilesetBlueprint.h"
#include "TilesetReference.h"
#include "LayerBlueprint.h"

namespace ni {

class Tilemap
{
private:
	TilemapBlueprint blueprint_;

	std::vector<TilesetBlueprint> tileset_blueprints_ = {};

	TilemapGraphicsComponent graphics_;

	std::string last_loaded_file_ = "";

	bool has_loaded_collision = false;
	bool has_loaded_graphics  = false;

	bool is_collision_enabled = false;

	void LoadTilesetBlueprints(const std::vector<TilesetReference>& tileset_references);
	void LoadTiles(const LayerBlueprint& layer, bool collision_enabled);

public:
	inline static const std::string kPrototypeLayerName = "prototype";

	Tilemap(const std::string& filepath);

	bool LoadFromFile(const std::string& filepath, bool collision_enabled = true);

	void Render(sf::RenderTarget& target, sf::RenderStates states);
};

}