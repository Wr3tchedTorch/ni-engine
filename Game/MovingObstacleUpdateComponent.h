#pragma once

#include "ObstacleUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>

class MovingObstacleUpdateComponent : public ObstacleUpdateComponent
{
public:
	MovingObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::TransformComponent& transform, ni::Id<ni::GameObjectTag> id, ni::Id<ni::GameObjectTag> player_id, sf::Vector2i position_offset, float delay_in_seconds);
	void Update() override;

private:
	sf::Vector2i position_offset_ = {};

	sf::Vector2f target_position_ = {};
	sf::Vector2f start_position_  = {};

	float    delay_in_seconds_ = 0;
	sf::Time time_since_movement_started_;

	bool moving_ = true;
};

