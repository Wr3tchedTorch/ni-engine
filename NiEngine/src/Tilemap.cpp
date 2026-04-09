#include <NiEngine/Tilemap.h>

#include <id.h>

#include <string>
#include <filesystem>
#include <vector>
#include <cassert>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <NiEngine/DataHandler.h>
#include <NiEngine/TilemapBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/FileUtility.h>
#include <NiEngine/TilesetReference.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/BitmapStore.h>

void ni::Tilemap::LoadTilesetBlueprints(const std::vector<TilesetReference>& tileset_references)
{
	for (auto& tileset_reference : tileset_references)
	{
		std::string tileset_filepath = std::filesystem::path(tileset_reference.file_source_).replace_extension(".json").string();
		tileset_filepath = FileUtility::RemoveRelativePaths(tileset_filepath);

		DataHandler<TilesetBlueprint> tileset_handler(tileset_filepath);
		TilesetBlueprint tileset_blueprint = tileset_handler.GetBlueprint();

		tileset_blueprint.texture_key_ = FileUtility::RemoveRelativePaths(tileset_blueprint.texture_key_);
		tileset_blueprint.first_gid_ = tileset_reference.first_gid_;

		tileset_blueprints_.emplace_back(tileset_blueprint);
	}
}

void ni::Tilemap::LoadTiles(const LayerBlueprint& layer_blueprint, bool collision_enabled)
{
	for (int y = 0; y < blueprint_.map_size_.y; ++y)
	{
		for (int x = 0; x < blueprint_.map_size_.x; ++x)
		{
			int index = x + y * blueprint_.map_size_.x;
			int tile_id = layer_blueprint.data_.at(index);
			if (tile_id == 0)
			{
				continue;
			}

			const TilesetBlueprint& tileset_blueprint = GetTilesetByGid(tileset_blueprints_, tile_id);
			tile_id -= tileset_blueprint.first_gid_;

			graphics_.AddTile({ x, y }, tile_id, tileset_blueprint, layer_blueprint.position_);
			collision_.AddTile({ x, y }, tile_id, tileset_blueprint, layer_blueprint, blueprint_.map_size_, blueprint_.tile_size_);
		}
	}
	collision_.CreateCollision();
}

ni::Tilemap::Tilemap(b2WorldId world_id) : collision_(world_id)
{
}

bool ni::Tilemap::LoadFromFile(const std::string& filepath, bool collision_enabled)
{
	DataHandler<TilemapBlueprint> handler(filepath);

	blueprint_ = handler.GetBlueprint();

	LoadTilesetBlueprints(blueprint_.tileset_references_);

	for (int i = 0; i < blueprint_.layers_.size(); ++i)
	{
		LayerBlueprint& layer_blueprint = blueprint_.layers_[i];
		if (layer_blueprint.name_ == kPrototypeLayerName)
		{
			continue;
		}
		LoadTiles(layer_blueprint, collision_enabled);
	}

	return true;
}

sf::FloatRect ni::Tilemap::GetBounds() const
{
	return graphics_.GetBounds(blueprint_.map_size_, blueprint_.tile_size_);
}

void ni::Tilemap::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	graphics_.Render(target, states, store);
}

const ni::TilesetBlueprint& ni::Tilemap::GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid)
{
	const TilesetBlueprint* result = nullptr;
	for (auto& tileset : tileset_blueprints)
	{
		if (gid >= tileset.first_gid_)
		{
			result = &tileset;
		}
	}
	assert(result != nullptr);
	return *result;
}
