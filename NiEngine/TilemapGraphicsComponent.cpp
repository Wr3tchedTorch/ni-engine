#include "TilemapGraphicsComponent.h"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "TilemapBlueprint.h"
#include "LayerBlueprint.h"
#include "TilesetBlueprint.h"

void ni::TilemapGraphicsComponent::UpdateVertices(int index, TilemapBlueprint& blueprint, std::vector<TilesetBlueprint>& tileset_blueprints)
{
	LayerBlueprint & layer_blueprint = blueprint.layers_.at(index);	
	sf::VertexArray& layer_vertices  = layers_vertices_.at(index);

	layers_vertices_.resize(blueprint.map_size_.x * blueprint.map_size_.y * kVerticesPerTile);
	for (int y = 0; y < blueprint.map_size_.y; ++y)
	{
		for (int x = 0; x < blueprint.map_size_.x; ++x)
		{
			int index = x + y * blueprint.map_size_.x;

			int tile_id = layer_blueprint.data_.at(index);

			float top  = y * blueprint.tile_size_.y;
			float left = x * blueprint.tile_size_.x;
			float bottom = top  + blueprint.tile_size_.y;
			float right  = left + blueprint.tile_size_.x;
		}
	}
}

const ni::TilesetBlueprint& ni::TilemapGraphicsComponent::GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid)
{
	int lowest_difference = 0;
	bool found = false;

	const TilesetBlueprint* resulting_tileset;
	for (auto& tileset : tileset_blueprints)
	{
		int result = gid - tileset.first_gid_;
		if (result < 0)
		{			
			break;
		}
		if (!found || lowest_difference > result)
		{
			resulting_tileset = &tileset;
			lowest_difference = result;
			found = true;
		}
		if (found && lowest_difference < result)
		{
			break;
		}
	}

	return *resulting_tileset;
}

void ni::TilemapGraphicsComponent::AddTile(const sf::Vector2i& grid_position, int tile_id, const std::vector<TilesetBlueprint>& tileset_blueprints)
{
	int x = grid_position.x;
	int y = grid_position.y;

	float top  = y * tileset_blueprints.tile_size_.y;
	float left = x * tileset_blueprints.tile_size_.x;
	float bottom = top + tileset_blueprints.tile_size_.y;
	float right = left + tileset_blueprints.tile_size_.x;
}

void ni::TilemapGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
}
