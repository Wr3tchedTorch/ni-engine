#pragma once

#include <vector>

#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>

using json = nlohmann::json;

namespace ni {

	struct PolygonBlueprint
	{
		int id_ = 0;

		sf::Vector2i position_;
		std::vector<sf::Vector2i> offset_points_;
	};

	inline void to_json(json& j, const PolygonBlueprint& pb)
	{
		j =
		{
			{"id", pb.id_},
			{"x",  pb.position_.x},
			{"y",  pb.position_.y}
		};

		for (const auto& points : pb.offset_points_)
		{
			j["polygon"] += {
				{ "x", points.x },
				{ "y", points.y }
			};
		}
	}

	inline void from_json(const json& j, PolygonBlueprint& pb)
	{
		j.at("id").get_to(pb.id_);
		j.at("x").get_to(pb.position_.x);
		j.at("y").get_to(pb.position_.y);

		for (auto& point_json : j.at("polygon"))
		{
			sf::Vector2i point{};

			point_json.at("x").get_to(point.x);
			point_json.at("y").get_to(point.y);

			pb.offset_points_.push_back(point);
		}
	}

}