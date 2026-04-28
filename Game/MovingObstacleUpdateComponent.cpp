#include "MovingObstacleUpdateComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Engine.h>

#include "ObstacleUpdateComponent.h"

MovingObstacleUpdateComponent::MovingObstacleUpdateComponent(
	ni::ComponentLocator& component_locator, 
	ni::TransformComponent& transform,
	ni::Id<ni::GameObjectTag> id, 
	ni::Id<ni::GameObjectTag> player_id, 
	sf::Vector2i position_offset, 
	float delay_in_seconds
) : ObstacleUpdateComponent(component_locator, id, player_id)
{
	delay_in_seconds_ = delay_in_seconds;
	position_offset_  = position_offset;
	
	sf::Vector2f current_position = transform.GetTransformable().getPosition();
	target_position_ = current_position + sf::Vector2f(position_offset_);	
	start_position_  = current_position;

	time_since_movement_started_ = ni::Engine::time_elapsed;
}

void MovingObstacleUpdateComponent::Update()
{
	// MOVEMENT IMPLEMENTATION

	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);
	
	float time_passed = (ni::Engine::time_elapsed - time_since_movement_started_).asSeconds();
	if (moving_ && target_position_ != transform->GetTransformable().getPosition() && time_passed < delay_in_seconds_)
	{
		sf::Vector2f new_pos = transform->GetTransformable().getPosition();

		new_pos.x = std::lerp(start_position_.x, target_position_.x, time_passed / delay_in_seconds_);
		new_pos.y = std::lerp(start_position_.y, target_position_.y, time_passed / delay_in_seconds_);

		transform->GetTransformable().setPosition(new_pos);
	}
	
	ObstacleUpdateComponent::Update();
}
