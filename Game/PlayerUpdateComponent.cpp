#include "PlayerUpdateComponent.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/PlatformerCharacterPhysicsComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>

PlayerUpdateComponent::PlayerUpdateComponent(ni::Id<GameObjectTag> owner_id, ni::ComponentLocator& component_locator) : ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;
	
	ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([this](const sf::Event::KeyPressed& event)
	{
		if (event.scancode == sf::Keyboard::Scancode::Space && physics_component_)
		{
			physics_component_->Jump();
		}
	});	
}

void PlayerUpdateComponent::Update()
{
	if (!physics_component_)
	{
		physics_component_ = static_cast<ni::PlatformerCharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
		return;
	}

	float throttle = (int)(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) - (int)(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A));

	if (throttle != 0)
	{
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		
		graphics->PlayAnimation(12, .2, true);
	}

	physics_component_->Move(throttle);
}
