#include "PlayerUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "CharacterPhysicsComponent.h"

PlayerUpdateComponent::PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) : ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;

	ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([this](const sf::Event::KeyPressed& event) {
		if (event.scancode == sf::Keyboard::Scancode::Space)
		{
			Jump();
		}
	});
}

void PlayerUpdateComponent::Update()
{
	int dir = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);

	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (physics)
	{
		physics->Move(dir);
	}
}

void PlayerUpdateComponent::Jump()
{
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (physics)
	{
		physics->Jump();
	}
}
