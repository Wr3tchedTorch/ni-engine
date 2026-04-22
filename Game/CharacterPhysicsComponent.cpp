#include "CharacterPhysicsComponent.h"

#include <id.h>

#include <iostream>
#include <cstdlib>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/MathUtility.h>
#include <NiEngine/ServiceLocator.h>

CharacterPhysicsComponent::CharacterPhysicsComponent(sf::Vector2i character_size) : size_(character_size)
{
	ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([this](const sf::Event::KeyPressed& event)
	{
		if (event.scancode == sf::Keyboard::Scancode::Space)
		{
			Jump();
		}
	});
}

void CharacterPhysicsComponent::PhysicsUpdate(ni::TransformComponent& transform_component, b2WorldId _, const ni::Tilemap* current_tilemap)
{
	if (!current_tilemap)
	{
		std::cout << "ERROR! Tilemap not provided!";
		return;
	}

	Move(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A));
	if (is_falling_)
	{
		velocity_.y += GRAVITY / 10.0f;
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
	transform_component.GetTransformable().move(velocity_);

	HandleCollisions(transform_component, current_tilemap);
}

void CharacterPhysicsComponent::Move(float dir)
{
	velocity_.x = dir * speed_ / 10.0f;
}

void CharacterPhysicsComponent::Jump()
{
	if (is_falling_)
	{
		return;
	}
	velocity_.y = -jump_force_;
	is_falling_ = true;
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

	is_falling_ = true;
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

			if (collision_block.findIntersection(GetHeadBounds(transform_component.GetTransformable().getPosition())))
			{
				velocity_.y = 0;

				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();

				snap_position.y = collision_block.position.y + collision_block.size.y + size_.y / 2.0f + 1;

				transform_component.GetTransformable().setPosition(snap_position);
			}
			if (collision_block.findIntersection(GetFeetBounds(transform_component.GetTransformable().getPosition())))
			{
				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();
				snap_position.y = collision_block.position.y - size_.y / 2.0f;

				transform_component.GetTransformable().setPosition(snap_position);

				is_falling_ = false;
			}
			if (collision_block.findIntersection(GetFrontBounds(transform_component.GetTransformable().getPosition())))
			{
				sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();

				int movement_sign = ni::MathUtility::GetSign(velocity_.x);

				float block_center_x = collision_block.position.x + collision_block.size.x / 2.0f;
				snap_position.x = block_center_x + size_.x * -movement_sign;

				transform_component.GetTransformable().setPosition(snap_position);
			}
		}
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

	return sf::FloatRect();
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
