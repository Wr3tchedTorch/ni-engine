#pragma once
#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

struct ObjectBlueprint
{
	std::string  template_filepath_ = "";
	sf::Vector2f position_ = {};
};

inline void to_json(json& j, const ObjectBlueprint& lb)
{
	j =
	{
		{"x", lb.position_.x},
		{"y", lb.position_.y},
		{"template", lb.template_filepath_},
	};
}

inline void from_json(const json& j, ObjectBlueprint& lb)
{
	j.at("x").get_to(lb.position_.x);
	j.at("y").get_to(lb.position_.y);
	j.at("template").get_to(lb.template_filepath_);
}

}
