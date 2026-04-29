#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>
#include <NiEngine/FileUtility.h>

#include "ObstacleObjectBlueprint.h"
#include <filesystem>
#include <format>

using json = nlohmann::json;

enum ObjectType
{
	Obstacle = 1,
};

struct ObjectLayerBlueprint
{
	std::string name_ = "";
	sf::Vector2f position_ = {};

	std::vector<ObstacleObjectBlueprint*> obstacles_ = {};
};

inline void to_json(json& j, const ObjectLayerBlueprint& lb)
{
	j =
	{
		{"x", lb.position_.x},
		{"y", lb.position_.y},
		{"name", lb.name_},
	};

	json objects_json = json::array();
	for (auto obstacle : lb.obstacles_)
	{
		objects_json.push_back(*obstacle);
	}

	j["objects"] = objects_json;
}

inline void from_json(const json& j, ObjectLayerBlueprint& lb)
{
	j.at("x").get_to(lb.position_.x);
	j.at("y").get_to(lb.position_.y);
	j.at("name").get_to(lb.name_);

	for (int i = 0; i < j.at("objects").size(); ++i)
	{
		auto& object_json = j.at("objects").at(i);		

		std::string template_filepath = std::filesystem::path(static_cast<std::string>(object_json.at("template"))).replace_extension(".json").string();
		template_filepath = ni::FileUtility::RemoveRelativePaths(template_filepath);
		template_filepath = std::format("maps/{}", template_filepath);


	
		if (j.at("type_id") == Obstacle)
		{			
		}
	}

	for (auto& object : j.at("objects"))
	{
	}
}
