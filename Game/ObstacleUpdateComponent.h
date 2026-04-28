#pragma once

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/TransformComponent.h>
#include <SFML/Graphics/Rect.hpp>

#include "CharacterPhysicsComponent.h"

class ObstacleUpdateComponent : public ni::UpdateComponent
{
public:
	ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, ni::Id<ni::GameObjectTag> player_id);
	
	void Update() override;

protected:
	ni::Id<ni::GameObjectTag> player_id_;
	
	bool harmful_   = false;
	bool collidable = false;

	virtual void CollideTop	  (sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics);
	virtual void CollideBottom(sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics);
	virtual void CollideFront (sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics);

private:


	void HandleCollisions();
};

