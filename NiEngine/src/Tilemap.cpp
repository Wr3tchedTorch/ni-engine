#include <NiEngine/Tilemap.h>

#include <id.h>

#include <string>
#include <filesystem>
#include <vector>
#include <cassert>
#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <NiEngine/DataHandler.h>
#include <NiEngine/TilemapBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/FileUtility.h>
#include <NiEngine/TilesetReference.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/TileBlueprint.h>
#include <NiEngine/TilemapCollisionComponent.h>

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

void ni::Tilemap::LoadTiles(const LayerBlueprint& layer_blueprint)
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

			if (collision_)
			{
				collision_->AddTile({ x, y }, tile_id, tileset_blueprint, layer_blueprint, blueprint_.map_size_, blueprint_.tile_size_);
			}
		}
	}
	if (collision_)
	{
		collision_->CreateCollision();
	}
}

void ni::Tilemap::EnableCollision(b2WorldId world_id)
{
	collision_ = std::make_unique<TilemapCollisionComponent>(world_id);
}


bool ni::Tilemap::LoadFromFile(const std::string& filepath)
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
		LoadTiles(layer_blueprint);
	}

	return true;
}

void ni::Tilemap::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	graphics_.Render(target, states, store);
}

sf::FloatRect ni::Tilemap::GetBounds() const
{
	return graphics_.GetBounds(blueprint_.map_size_, blueprint_.tile_size_);
}

sf::Vector2i ni::Tilemap::GlobalToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i result;
	result.x = position.x / blueprint_.tile_size_.x;
	result.y = position.y / blueprint_.tile_size_.y;
	return result;
}

ni::TileBlueprint ni::Tilemap::GetTileInfo(sf::Vector2i tile_grid_position, const std::string& layer_name) const
{
	bool out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= blueprint_.map_size_.x ||
						 tile_grid_position.y < 0 || tile_grid_position.y >= blueprint_.map_size_.y;
	if (out_of_bounds)
	{
		return TileBlueprint();
	}

	const LayerBlueprint* layer = GetLayerByName(layer_name);
	if (!layer)
	{
		return TileBlueprint();
	}

	int tile_index = tile_grid_position.x + tile_grid_position.y * blueprint_.map_size_.x;
	if (tile_index > 0 && tile_index < (int)layer->data_.size() && layer->data_[tile_index] != 0)
	{
		int tile_gid = layer->data_[tile_index];
		const TilesetBlueprint& tileset = GetTilesetByGid(tileset_blueprints_, tile_gid);

		auto it = tileset.tiles_.find(tile_gid - tileset.first_gid_);
		if (it != tileset.tiles_.end())
		{
			return tileset.tiles_.at(tile_gid - tileset.first_gid_);
		}
	}

	return TileBlueprint();
}

const ni::LayerBlueprint* ni::Tilemap::GetLayerByName(const std::string& layer_name) const
{
	for (auto& layer : blueprint_.layers_)
	{
		if (layer.name_ == layer_name)
		{
			return &layer;
		}
	}
	return nullptr;
}

bool ni::Tilemap::IsTileEmpty(sf::Vector2i tile_grid_position) const
{
	bool out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= blueprint_.map_size_.x ||
						 tile_grid_position.y < 0 || tile_grid_position.y >= blueprint_.map_size_.y;

	if (out_of_bounds)
	{
		return true;
	}

	for (auto& layer : blueprint_.layers_)
	{
		if (layer.name_ == kPrototypeLayerName)
		{
			continue;
		}

		int tile_index = tile_grid_position.x + tile_grid_position.y * blueprint_.map_size_.x;

		if (tile_index > 0 && tile_index < (int)layer.data_.size() && layer.data_[tile_index] != 0)
		{
			return false;
		}
	}
	return true;
}

bool ni::Tilemap::IsTileEmpty(const std::vector<int>& map, sf::Vector2i map_size, sf::Vector2i tile_grid_position)
{
	bool is_out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= map_size.x ||
		tile_grid_position.y < 0 || tile_grid_position.y >= map_size.y;
	if (is_out_of_bounds)
	{
		return true;
	}
	int tile_index = tile_grid_position.x + tile_grid_position.y * map_size.x;

	return tile_index < 0 || tile_index >= (int)map.size() || map[tile_index] == 0;
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
