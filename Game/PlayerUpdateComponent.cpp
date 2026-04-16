#include "PlayerUpdateComponent.h"

#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/PlatformerCharacterPhysicsComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <NiEngine/ComponentLocator.h>

PlayerUpdateComponent::PlayerUpdateComponent(ni::Id<GameObjectTag> owner_id, ni::ComponentLocator& component_locator) : ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;
	
	auto physics_comp = static_cast<ni::PlatformerCharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([physics_comp](const sf::Event::KeyPressed& event) 
	{
		if (event.scancode == sf::Keyboard::Scancode::Space)
		{
			physics_comp->Jump();
		}
	});
}

void PlayerUpdateComponent::Update()
{
	auto physics_comp = static_cast<ni::PlatformerCharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (!physics_comp)
	{
		return;
	}

	float throttle = (int)(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) - (int)(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A));

	physics_comp->SolveMove(throttle);
}
