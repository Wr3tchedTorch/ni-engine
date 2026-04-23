#pragma once

#include <functional>

#include <NiEngine/TransformComponent.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/Subject.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class CharacterPhysicsComponent : public ni::PhysicsComponent
{
public:
	inline static const float GRAVITY = 20.0f;
	
	CharacterPhysicsComponent(sf::Vector2i character_size);
	void PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap, float delta) override;

	void Move(float dir);
	void Jump();

	int OnFalling(std::function<void()> callback) { return on_falling_.Subscribe(callback); }
	int OnJumping(std::function<void()> callback) { return on_jumping_.Subscribe(callback); }
	int OnLanding(std::function<void()> callback) { return on_landing_.Subscribe(callback); }

private:
	ni::Subject<> on_falling_;
	ni::Subject<> on_jumping_;
	ni::Subject<> on_landing_;

	sf::Vector2f velocity_;
	sf::Vector2i size_;

	float speed_ = 100;
	float jump_force_ = 400;
	
	bool is_jumping_ = true;
	bool is_falling_ = true;
	
	void HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap);

	sf::FloatRect GetFeetBounds(sf::Vector2f position) const;
	sf::FloatRect GetHeadBounds(sf::Vector2f position) const;
	sf::FloatRect GetFrontBounds(sf::Vector2f position) const;
};

