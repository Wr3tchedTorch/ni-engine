#pragma once

#include <nlohmann/json.hpp>
#include <NiEngine/PolygonBlueprint.h>

using json = nlohmann::json;

namespace ni {

struct TileBlueprint
{
	int id_ = 0;

	bool is_hill_ = false;
	bool one_sided_collision_ = false;

	PolygonBlueprint polygon_blueprint_;
};

inline void to_json(json& j, const TileBlueprint& tb)
{
	j =
	{
		{"id", tb.id_}
	};

	j["properties"] += 
	{ 
		{"name", "oneSidedCollision"},
		{"value"}, tb.one_sided_collision_
	};

	j["properties"] +=
	{
		{"name", "isHill"},
		{ "value" }, tb.is_hill_
	};	

	if (tb.polygon_blueprint_.id_ > 0)
	{
		j["objectgroup"]["objects"].push_back(tb.polygon_blueprint_);
	}
}

inline void from_json(const json& j, TileBlueprint& tb)
{
	j.at("id").get_to(tb.id_);

	if (j.contains("objectgroup") && j.at("objectgroup").contains("objects") && j.at("objectgroup").at("objects").size() > 0)
	{
		for (auto& polygon_json : j.at("objectgroup").at("objects"))
		{
			tb.polygon_blueprint_ = polygon_json;
		}
	}

	if (!j.contains("properties"))
	{
		return;
	}

	for (auto& property : j.at("properties"))
	{
		if (property.at("name") == "oneSidedCollision")
		{
			property.at("value").get_to(tb.one_sided_collision_);
		}
		else if (property.at("name") == "isHill")
		{
			property.at("value").get_to(tb.is_hill_);
		}
	}
}

}