#include <NiEngine/TilemapCollisionComponent.h>

#include <box2d.h>
#include <types.h>
#include <math_functions.h>
#include <id.h>

#include <unordered_map>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/TileBlueprint.h>

bool ni::TilemapCollisionComponent::IsTileEmpty(const std::vector<int>& map, sf::Vector2i map_size, sf::Vector2i tile_grid_position)
{
	if (tile_grid_position.x < 0 || tile_grid_position.x >= map_size.x ||
		tile_grid_position.y < 0 || tile_grid_position.y >= map_size.y)
	{
		return true;
	}
	return IsTileEmpty(map, tile_grid_position.x + tile_grid_position.y * map_size.x);
}

bool ni::TilemapCollisionComponent::IsTileEmpty(const std::vector<int>& map, int tile_index)
{
	return tile_index < 0 || tile_index >= (int)map.size() || map[tile_index] == 0;
}

std::vector<ni::LoopInformation> ni::TilemapCollisionComponent::GetCollisionLoops(sf::Vector2i tile_size)
{
	std::vector<LoopInformation> loops;

	while (!exposed_edges_.empty())
	{
		sf::Vector2i starting_point = exposed_edges_.begin()->first;
		sf::Vector2i current_point  = starting_point;

		bool is_closed = true;
		std::vector<b2Vec2> loop;
		do
		{
			loop.push_back(Converter::PixelsToMeters(sf::Vector2i({ current_point.x * tile_size.x, current_point.y * tile_size.y })));

			auto it = exposed_edges_.find(current_point);
			if (it == exposed_edges_.end())
			{
				is_closed = false;
				break;
			}

			sf::Vector2i nextPoint = it->second;

			exposed_edges_.erase(current_point);
			current_point = nextPoint;

		} while (current_point != starting_point);

		loops.push_back({ is_closed, loop });
	}

	return loops;
}

void ni::TilemapCollisionComponent::CreateOnesidedCollision(sf::Vector2i tile_size)
{
	std::swap(exposed_edges_, one_sided_edges_);
	std::vector<LoopInformation> one_sided_loops = GetCollisionLoops(tile_size);
	std::swap(exposed_edges_, one_sided_edges_);

	for (const auto& loop : one_sided_loops)
	{
		std::vector<b2Vec2> padded;
		padded.reserve(loop.data_.size() + 2);

		b2Vec2 ghost_start = {
			loop.data_[0].x - (loop.data_[0].x - loop.data_[1].x),
			loop.data_[0].y - (loop.data_[0].y - loop.data_[1].y)
		};
		padded.push_back(ghost_start);

		for (const auto& p : loop.data_)
			padded.push_back(p);

		b2Vec2 ghost_end = {
			loop.data_.back().x - (loop.data_[loop.data_.size() - 2].x - loop.data_.back().x),
			loop.data_.back().y - (loop.data_[loop.data_.size() - 2].y - loop.data_.back().y)
		};
		padded.push_back(ghost_end);

		b2ChainDef shape = b2DefaultChainDef();
		shape.points = padded.data();
		shape.count = padded.size();
		shape.isLoop = false;
		b2CreateChain(body_id_, &shape);
	}
}

ni::TilemapCollisionComponent::TilemapCollisionComponent(b2WorldId world_id)
{
	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.type = b2_staticBody;

	body_id_ = b2CreateBody(world_id, &body_def);
}

void ni::TilemapCollisionComponent::AddTile(sf::Vector2i grid_position, int tile_gid, const TilesetBlueprint& tileset, const LayerBlueprint& layer, sf::Vector2i map_size)
{
	int gx = grid_position.x;
	int gy = grid_position.y;

	sf::Vector2i top	= { gx,		gy - 1 };
	sf::Vector2i left   = { gx - 1, gy	   };
	sf::Vector2i right	= { gx + 1, gy	   };
	sf::Vector2i bottom = { gx,		gy + 1 };

	const TileBlueprint& tile = tileset.tiles_.at(tile_gid);

	bool is_top_empty = IsTileEmpty(layer.data_, map_size, top);
	if (is_top_empty && tile.one_sided_collision_)
	{
		one_sided_edges_[{gx, gy}] = { gx + 1, gy };
		return;
	}

	if (is_top_empty)
	{
		exposed_edges_[{gx, gy}] = { gx + 1, gy };
	}
	if (IsTileEmpty(layer.data_, map_size, bottom))
	{
		exposed_edges_[{gx + 1, gy + 1}] = { gx, gy + 1 };
	}
	if (IsTileEmpty(layer.data_, map_size, left))
	{
		exposed_edges_[{gx, gy + 1}] = { gx, gy };
	}
	if (IsTileEmpty(layer.data_, map_size, right))
	{
		exposed_edges_[{gx + 1, gy}] = { gx + 1, gy + 1 };
	}
}

void ni::TilemapCollisionComponent::CreateCollision(sf::Vector2i tile_size)
{	
	std::vector<LoopInformation> loops = GetCollisionLoops(tile_size);

	for (const auto& loop : loops)
	{
		b2ChainDef shape = b2DefaultChainDef();

		shape.points = loop.data_.data();
		shape.count  = loop.data_.size();
		shape.isLoop = loop.is_closed_;

		b2CreateChain(body_id_, &shape);
	}

	CreateOnesidedCollision(tile_size);
}
