#include "TilemapGraphicsComponent.h"

#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include "TilesetBlueprint.h"
#include "BitmapStore.h"

const ni::TilesetBlueprint& ni::TilemapGraphicsComponent::GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid)
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

void ni::TilemapGraphicsComponent::AddTile(const sf::Vector2i& grid_position, int tile_id, const std::vector<TilesetBlueprint>& tileset_blueprints)
{
	int x = grid_position.x;
	int y = grid_position.y;

	const TilesetBlueprint& tileset = GetTilesetByGid(tileset_blueprints, tile_id);

	float top    = y * tileset.tile_size_.y;
	float left   = x * tileset.tile_size_.x;
	float bottom = top  + tileset.tile_size_.y;
	float right  = left + tileset.tile_size_.x;

	int local_id = tile_id - tileset.first_gid_;

	float uv_x = local_id % tileset.columns_;
	float uv_y = std::floor(static_cast<float>(local_id) / tileset.columns_);

	float uv_top    = uv_y    * (tileset.tile_size_.y + tileset.spacing_) + tileset.margin_;
	float uv_left   = uv_x    * (tileset.tile_size_.x + tileset.spacing_) + tileset.margin_;
	float uv_bottom = uv_top  + tileset.tile_size_.y;
	float uv_right  = uv_left + tileset.tile_size_.x;

	vertices_by_tileset_[tileset.texture_key_].setPrimitiveType(sf::PrimitiveType::Triangles);

	vertices_by_tileset_[tileset.texture_key_].append({ {left,  top   }, sf::Color::White, {uv_left,  uv_top   } });
	vertices_by_tileset_[tileset.texture_key_].append({ {right, top   }, sf::Color::White, {uv_right, uv_top   } });
	vertices_by_tileset_[tileset.texture_key_].append({ {left,  bottom}, sf::Color::White, {uv_left,  uv_bottom} });

	vertices_by_tileset_[tileset.texture_key_].append({ {right, top   }, sf::Color::White, {uv_right, uv_top   } });
	vertices_by_tileset_[tileset.texture_key_].append({ {right, bottom}, sf::Color::White, {uv_right, uv_bottom} });
	vertices_by_tileset_[tileset.texture_key_].append({ {left,  bottom}, sf::Color::White, {uv_left,  uv_bottom} });
}

void ni::TilemapGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	for (const auto& [texture_key, vertices] : vertices_by_tileset_)
	{		
		sf::Texture texture(store.GetTexture(texture_key));
		states.texture = &texture;
		target.draw(vertices, states);
	}
}
