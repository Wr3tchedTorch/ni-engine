#pragma once

#include <NiEngine/TransformComponent.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/Tilemap.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class CharacterPhysicsComponent : public ni::PhysicsComponent
{
public:
	inline static const float GRAVITY = 4.0f;

	CharacterPhysicsComponent(sf::Vector2i character_size);
	void PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap) override;

	void Move(float dir);
	void Jump();

private:
	sf::Vector2f velocity_;
	sf::Vector2i size_;

	float speed_ = 16;
	float jump_force_ = 10;
	
	bool is_jumping_ = true;
	bool is_falling_ = true;
	
	void HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap);

	sf::FloatRect GetFeetBounds(sf::Vector2f position) const;
	sf::FloatRect GetHeadBounds(sf::Vector2f position) const;
	sf::FloatRect GetFrontBounds(sf::Vector2f position) const;
};

