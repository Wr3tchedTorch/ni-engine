#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/Subject.h>
#include <NiEngine/TileBlueprint.h>

enum CharacterState
{
	Jumping,
	Falling,
	Idle,
	Moving
};

class CharacterPhysicsComponent : public ni::PhysicsComponent
{
public:
	inline static const float GRAVITY = 20.0f;
	
	CharacterPhysicsComponent(sf::Vector2i character_size);
	void PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap, float delta) override;

	void Move(float dir);
	void Jump();
	void FallFromPlatform();

	int OnFalling(std::function<void()> callback) { return on_falling_.Subscribe(callback); }
	int OnJumping(std::function<void()> callback) { return on_jumping_.Subscribe(callback); }
	int OnLanding(std::function<void()> callback) { return on_landing_.Subscribe(callback); }

	sf::FloatRect GetFeetBounds(sf::Vector2f position) const;
	sf::FloatRect GetHeadBounds(sf::Vector2f position) const;
	sf::FloatRect GetFrontBounds(sf::Vector2f position) const;

private:
	ni::Subject<> on_falling_;
	ni::Subject<> on_jumping_;
	ni::Subject<> on_landing_;

	sf::Vector2f velocity_;
	sf::Vector2i size_;

	float speed_ = 100;
	float jump_force_ = 400;
	
	CharacterState state_ = CharacterState::Falling;

	bool fall_through_platform_    = false;
	bool falling_through_platform_ = false;
	
	void HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap* current_tilemap);

	bool CollideTop(   ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
	bool CollideBottom(ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
	bool CollideFront( ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
};

