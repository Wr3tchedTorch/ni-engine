#include <NiEngine/TilemapGraphicsComponent.h>

#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/BitmapStore.h>

void ni::TilemapGraphicsComponent::AddTile(const sf::Vector2i& grid_position, int tile_id, const TilesetBlueprint& tileset, const sf::Vector2f& layer_position_offset)
{
	int x = grid_position.x;
	int y = grid_position.y;

	float top    = y * tileset.tile_size_.y;
	float left   = x * tileset.tile_size_.x;
	float bottom = top  + tileset.tile_size_.y;
	float right  = left + tileset.tile_size_.x;

	int local_id = tile_id;

	float uv_x = local_id % tileset.columns_;
	float uv_y = std::floor(static_cast<float>(local_id) / tileset.columns_);

	float uv_top    = uv_y    * (tileset.tile_size_.y + tileset.spacing_) + tileset.margin_;
	float uv_left   = uv_x    * (tileset.tile_size_.x + tileset.spacing_) + tileset.margin_;
	float uv_bottom = uv_top  + tileset.tile_size_.y;
	float uv_right  = uv_left + tileset.tile_size_.x;

	if (tileset_drawables_[tileset.texture_key_].vertices_.getVertexCount() == 0)
	{
		tileset_drawables_[tileset.texture_key_].position_offset_ = layer_position_offset;
		tileset_drawables_[tileset.texture_key_].vertices_.setPrimitiveType(sf::PrimitiveType::Triangles);
	}

	tileset_drawables_[tileset.texture_key_].vertices_.append({ {left,  top   }, sf::Color::White, {uv_left,  uv_top   } });
	tileset_drawables_[tileset.texture_key_].vertices_.append({ {right, top   }, sf::Color::White, {uv_right, uv_top   } });
	tileset_drawables_[tileset.texture_key_].vertices_.append({ {left,  bottom}, sf::Color::White, {uv_left,  uv_bottom} });

	tileset_drawables_[tileset.texture_key_].vertices_.append({ {right, top   }, sf::Color::White, {uv_right, uv_top   } });
	tileset_drawables_[tileset.texture_key_].vertices_.append({ {right, bottom}, sf::Color::White, {uv_right, uv_bottom} });
	tileset_drawables_[tileset.texture_key_].vertices_.append({ {left,  bottom}, sf::Color::White, {uv_left,  uv_bottom} });
}

void ni::TilemapGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	for (const auto& [texture_key, tileset_drawable] : tileset_drawables_)
	{
		sf::RenderStates local_state = states;		
		local_state.texture = &store.GetTexture(texture_key);		
		local_state.transform.translate(tileset_drawable.position_offset_);
		target.draw(tileset_drawable.vertices_, local_state);
	}
}

sf::FloatRect ni::TilemapGraphicsComponent::GetBounds() const
{
	sf::FloatRect result = {};
	result.position = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

	for (const auto& [texture_key, tileset_drawable] : tileset_drawables_)
	{
		sf::Vector2f vertices_size = tileset_drawable.vertices_.getBounds().size;

		result.position.x = std::min(result.position.x, tileset_drawable.position_offset_.x);
		result.position.y = std::min(result.position.y, tileset_drawable.position_offset_.y);

		result.size.x = std::max(result.size.x, tileset_drawable.position_offset_.x + vertices_size.x);
		result.size.y = std::max(result.size.y, tileset_drawable.position_offset_.y + vertices_size.y);
	}

	result.size -= result.position;
	return result;
}