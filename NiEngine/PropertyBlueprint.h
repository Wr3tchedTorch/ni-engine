#pragma once

#include <string>
#include <variant>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

using PropertyValue = std::variant<std::string, float, int, bool>;

struct PropertyBlueprint
{
	std::string   name_  = "";
	std::string   type_  = "";
	PropertyValue value_ = {};
};

inline void to_json(json& j, const PropertyBlueprint& pb)
{
	j =
	{
		{ "name",  pb.name_  },
		{ "type",  pb.type_  }		
	};

	std::visit([&j](const auto& val)
	{
		j["value"] = val;
	}, pb.value_);
}

inline void from_json(const json& j, PropertyBlueprint& pb)
{
	j.at("name") .get_to(pb.name_);
	j.at("type") .get_to(pb.type_);
	
	if (pb.type_ == "float") pb.value_ = j.at("value").get<float>();
	if (pb.type_ == "int")   pb.value_ = j.at("value").get<int>  ();
	if (pb.type_ == "bool")  pb.value_ = j.at("value").get<bool> ();
	else                     pb.value_ = j.at("value").get<std::string>();
}

}