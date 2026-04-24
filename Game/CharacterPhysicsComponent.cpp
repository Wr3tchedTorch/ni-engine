#include "CharacterPhysicsComponent.h"

#include <iostream>
#include <cstdlib>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/MathUtility.h>
#include <NiEngine/TileBlueprint.h>

CharacterPhysicsComponent::CharacterPhysicsComponent(sf::Vector2i character_size) : size_(character_size)
{
}

void CharacterPhysicsComponent::PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap, float delta)
{
	if (!current_tilemap)
	{
		std::cout << "ERROR! Tilemap not provided!";
		return;
	}

	if (state_ == CharacterState::Falling || state_ == CharacterState::Jumping)
	{
		velocity_.y += GRAVITY;

		if (velocity_.y > 0 && state_ != CharacterState::Falling)
		{
			state_ = CharacterState::Falling;
			on_falling_.Notify();
		}
	}
	else
	{
		velocity_.y = 0;	
	}

	sf::Vector2f new_scale = transform_component.GetTransformable().getScale();
	
	int movement_sign = ni::MathUtility::GetSign(velocity_.x);
	if (movement_sign != 0)
	{
		new_scale.x = abs(new_scale.x) * movement_sign;
	}

	transform_component.GetTransformable().setScale(new_scale);
	transform_component.GetTransformable().move(velocity_ * delta);

	HandleCollisions(transform_component, current_tilemap);
}

void CharacterPhysicsComponent::Move(float dir)
{
	velocity_.x = dir * speed_;

	if (dir != 0 && state_ != CharacterState::Falling && state_ != CharacterState::Jumping)
	{
		state_ = CharacterState::Moving;
	}
}

void CharacterPhysicsComponent::Jump()
{
	if (state_ == CharacterState::Falling || state_ == CharacterState::Jumping)
	{
		return;
	}
	velocity_.y = -jump_force_;
	state_ = CharacterState::Jumping;

	on_jumping_.Notify();
}

void CharacterPhysicsComponent::HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap)
{
	sf::Vector2f character_position = transform_component.GetTransformable().getPosition();

	sf::FloatRect collision_area;
	collision_area.position.x = character_position.x - size_.x / 2.0f;
	collision_area.position.y = character_position.y - size_.y / 2.0f;
	collision_area.size = sf::Vector2f(size_);

	sf::FloatRect collision_block;
	collision_block.size = sf::Vector2f(size_);

	int start_x = collision_area.position.x / size_.x - 1;
	int start_y = collision_area.position.y / size_.y - 1;
	int end_x   = collision_area.position.x / size_.x + 1;
	int end_y   = collision_area.position.y / size_.y + 1;

	bool is_falling = true;	
	for (int x = start_x; x <= end_x; ++x)
	{
		for (int y = start_y; y <= end_y; ++y)
		{
			if (current_tilemap->IsTileEmpty({ x, y }))
			{
				continue;
			}

			collision_block.position.x = x * size_.x;
			collision_block.position.y = y * size_.y;
			
			ni::TileBlueprint tile = current_tilemap->GetTileInfo({ x, y }, 1);

			if (collision_block.findIntersection(GetHeadBounds(transform_component.GetTransformable().getPosition())) && !tile.one_sided_collision_)
			{
				velocity_.y = 0;

				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();

				snap_position.y = collision_block.position.y + collision_block.size.y + size_.y / 2.0f + 1;

				transform_component.GetTransformable().setPosition(snap_position);
			}

			if (collision_block.findIntersection(GetFeetBounds(transform_component.GetTransformable().getPosition())) && !(tile.one_sided_collision_ && velocity_.y < 0))
			{
				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();
				snap_position.y = collision_block.position.y - size_.y / 2.0f;

				transform_component.GetTransformable().setPosition(snap_position);

				is_falling = false;

				if (state_ == CharacterState::Falling)
				{
					on_landing_.Notify();
					state_ = CharacterState::Idle;
				}
			}

			if (collision_block.findIntersection(GetFrontBounds(transform_component.GetTransformable().getPosition())) && !tile.one_sided_collision_)
			{
				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();

				int movement_sign = ni::MathUtility::GetSign(velocity_.x);

				float block_center_x = collision_block.position.x + collision_block.size.x / 2.0f;
				snap_position.x = block_center_x + size_.x * -movement_sign;

				transform_component.GetTransformable().setPosition(snap_position);
			}			
		}
	}	

	if (is_falling && state_ != CharacterState::Falling)
	{
		state_ = CharacterState::Falling;
		on_falling_.Notify();
	}
}

sf::FloatRect CharacterPhysicsComponent::GetFeetBounds(sf::Vector2f position) const
{
	sf::FloatRect feet_bounds;
	feet_bounds.size.x = size_.x * 2.0f / 5.0f;
	feet_bounds.size.y = size_.y / 4.0f + 1;
	
	feet_bounds.position = position;
	feet_bounds.position.x -= size_.x / 5.0f;
	feet_bounds.position.y += size_.y / 4.0f;

	return feet_bounds;
}

sf::FloatRect CharacterPhysicsComponent::GetHeadBounds(sf::Vector2f position) const
{
	sf::FloatRect head_bounds;
	head_bounds.size.x = size_.x * 2.0f / 5.0f;
	head_bounds.size.y = size_.y / 4.0f;

	head_bounds.position = position;
	head_bounds.position.x -= size_.x / 5.0f;
	head_bounds.position.y -= (size_.y / 4.0f) * 2 + 1;

	return head_bounds;
}

sf::FloatRect CharacterPhysicsComponent::GetFrontBounds(sf::Vector2f position) const
{
	int movement_sign = ni::MathUtility::GetSign(velocity_.x);

	float width = size_.x / 4.0f + 1;

	sf::FloatRect front_bounds;
	front_bounds.size.x = width;
	front_bounds.size.y = size_.y / 2.0f;

	front_bounds.position = position;
	front_bounds.position.x += size_.x / 4.0f * movement_sign - (movement_sign < 0 ? width : 0);
	front_bounds.position.y -= size_.y / 4.0f;

	return front_bounds;
}
