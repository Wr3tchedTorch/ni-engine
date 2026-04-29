#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

struct LayerBlueprint
{
	std::string name_ = "";
	sf::Vector2f position_ = {};

	std::vector<int> data_ = {};
	bool visible_ = false;

	float opacity_ = 0;
};

inline void to_json(json& j, const LayerBlueprint& lb)
{
	j =
	{
		{"x", lb.position_.x},
		{"y", lb.position_.y},
		{"name", lb.name_},
		{"opacity", lb.opacity_},
		{"visible", lb.visible_},
		{"data", lb.data_}
	};
}

inline void from_json(const json& j, LayerBlueprint& lb)
{
	if (j.at("type") != "tilelayer")
	{
		return;
	}

	j.at("x").get_to(lb.position_.x);
	j.at("y").get_to(lb.position_.y);
	j.at("name").get_to(lb.name_);
	j.at("opacity").get_to(lb.opacity_);
	j.at("visible").get_to(lb.visible_);
	j.at("data").get_to(lb.data_);
}

}
