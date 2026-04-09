#pragma once

#include <string>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include <NiEngine/MapUtility.h>
#include <NiEngine/TileBlueprint.h>

using json = nlohmann::json;

namespace ni {

struct TilesetBlueprint
{
	std::string name_ = "";
	std::string texture_key_ = "";

	sf::Vector2i tile_size_  = {};
	sf::Vector2i image_size_ = {};

	int spacing_	= 0;
	int margin_		= 0;
	int columns_	= 0;
	int tile_count_ = 0;

	std::unordered_map<int, TileBlueprint> tiles_ = {};

	int first_gid_ = 0;
};

inline void to_json(json& j, const TilesetBlueprint& tb)
{
	j =
	{
		{"name",	   tb.name_},
		{"image",      tb.texture_key_},
		{"tilewidth",  tb.tile_size_.x },
		{"tileheight", tb.tile_size_.y },
		{"imagewidth",  tb.image_size_.x },
		{"imageheight", tb.image_size_.y },
		{"spacing",    tb.spacing_ },
		{"margin",     tb.margin_ },
		{"columns",    tb.columns_},
		{"tilecount",  tb.tile_count_ },
		{"tiles",      MapUtility::MapToVector<int, TileBlueprint>(tb.tiles_)}
	};
}

inline void from_json(const json& j, TilesetBlueprint& tb)
{
	j.at("name").get_to(tb.name_);
	if (j.contains("image"))
	{
		j.at("image").get_to(tb.texture_key_);
		j.at("imagewidth").get_to(tb.image_size_.x);
		j.at("imageheight").get_to(tb.image_size_.y);
	}
	j.at("tilewidth").get_to(tb.tile_size_.x);
	j.at("tileheight").get_to(tb.tile_size_.y);
	j.at("spacing").get_to(tb.spacing_);
	j.at("margin").get_to(tb.margin_);
	j.at("columns").get_to(tb.columns_);
	j.at("tilecount").get_to(tb.tile_count_);

	if (!j.contains("tiles"))
	{
		return;
	}

	auto& tiles_json = j.at("tiles");
	for (const auto& tile : tiles_json)
	{
		TileBlueprint tile_blueprint = tile;
		tb.tiles_[tile_blueprint.id_] = tile_blueprint;
	}
}

}