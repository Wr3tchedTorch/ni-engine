#pragma once

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

struct PropertyBlueprint
{
	std::string name_  = "";
	std::string type_  = "";
	std::string value_ = "";
};

inline void to_json(json& j, const PropertyBlueprint& lb)
{
	j =
	{
		{ "name",  lb.name_  },
		{ "type",  lb.type_  },
		{ "value", lb.value_ }
	};
}

inline void from_json(const json& j, PropertyBlueprint& lb)
{
	j.at("name") .get_to(lb.name_);
	j.at("type") .get_to(lb.type_);
	j.at("value").get_to(lb.value_);
}

}