#include "ObstacleUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/TransformComponent.h>
#include <SFML/Graphics/Rect.hpp>

#include "CharacterPhysicsComponent.h"

ObstacleUpdateComponent::ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, ni::Id<ni::GameObjectTag> player_id) : ni::UpdateComponent(component_locator),
	player_id_(player_id)
{
	owner_id_ = id;
}

void ObstacleUpdateComponent::Update()
{
	HandleCollisions();
}

void ObstacleUpdateComponent::CollideTop(sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics)
{
	player_physics->CollideBottom(*player_transform, collision_box);
	player_physics->SetIsOnGround(true);
}

void ObstacleUpdateComponent::CollideBottom(sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics)
{
	player_physics->CollideTop(*player_transform, collision_box);
}

void ObstacleUpdateComponent::CollideFront(sf::FloatRect collision_box, ni::TransformComponent* player_transform, CharacterPhysicsComponent* player_physics)
{
	player_physics->CollideFront(*player_transform, collision_box);
}

void ObstacleUpdateComponent::HandleCollisions()
{
	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_ );
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));

	sf::FloatRect collision_box;
	collision_box.position = transform->GetTransformable().getPosition();
	collision_box.size     = { 16, 16 };

	if (collision_box.findIntersection(player_physics->GetFeetBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideTop(collision_box, player_transform, player_physics);
	}
	else
	{
		player_physics->SetIsOnGround(false);
	}

	if (collision_box.findIntersection(player_physics->GetFrontBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideFront(collision_box, player_transform, player_physics);
	}

	if (collision_box.findIntersection(player_physics->GetHeadBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideBottom(collision_box, player_transform, player_physics);
	}
}
