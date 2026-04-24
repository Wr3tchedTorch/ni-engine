#include "PlayerUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/SoundEngine.h>
#include <NiEngine/Animation.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "CharacterPhysicsComponent.h"

PlayerUpdateComponent::PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id)
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;

	ni::ServiceLocator::Instance().GetSoundEngine().Preload(kJumpSoundKey);
}

void PlayerUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([this](const sf::Event::KeyPressed& event) {
		if (event.scancode == sf::Keyboard::Scancode::Space)
		{
			Jump();
		}
	});

	physics.OnFalling([this]() {
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 4);
	});

	physics.OnJumping([this]() {
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 3);

		airborne_ = true;

		ni::ServiceLocator::Instance().GetSoundEngine().PlaySound(kJumpSoundKey);
	});

	physics.OnLanding([this]() {
		airborne_ = false;
	});

	ni::Animation jump_animation;
	jump_animation.key_ = kJumpAnimationKey;
	jump_animation.start_frame = 3;
	jump_animation.frame_count = 2;
	jump_animation.animation_row = 12;

	ni::Animation walk_animation;
	walk_animation.key_ = kWalkAnimationKey;
	walk_animation.start_frame = 1;
	walk_animation.frame_count = 2;
	walk_animation.animation_row = 12;

	graphics.RegisterAnimation(jump_animation);
	graphics.RegisterAnimation(walk_animation);
}

void PlayerUpdateComponent::Update()
{
	int dir = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);

	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{
		physics->Move(dir);
	}
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (!graphics || airborne_)
	{		
		return;
	}
	if (dir == 0)
	{
		graphics->SetFrame(kAnimationRow, 0);
		return;
	}
	graphics->Play(kWalkAnimationKey, .1, true);
}

void PlayerUpdateComponent::Jump()
{
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{
		physics->Jump();
	}
}
