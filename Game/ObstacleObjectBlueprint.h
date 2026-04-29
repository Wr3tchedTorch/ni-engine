#pragma once

#pragma once
#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ObstacleObjectBlueprint
{
	float		 movement_delay_      = 0;
	sf::Vector2f movement_offset_     = {};
	sf::IntRect  texture_coordinates_ = {};
	bool harmful_ = false;
};

inline void to_json(json& j, const ObstacleObjectBlueprint& lb)
{
	j =
	{
		{"x", lb.movement_offset_.x},
		{"y", lb.movement_offset_.y},
		{"movement_delay", lb.movement_delay_},
	};
}

inline void from_json(const json& j, ObstacleObjectBlueprint& lb)
{
	j.at("x").get_to(lb.movement_offset_.x);
	j.at("y").get_to(lb.movement_offset_.y);
	j.at("movement_delay").get_to(lb.movement_delay_);
}
