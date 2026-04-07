#pragma once

#include <unordered_map>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Id.h"
#include "GameObjectTag.h"
#include "GraphicsComponent.h"
#include "UpdateComponent.h"
#include "PhysicsComponent.h"
#include "BitmapStore.h"
#include "TransformComponent.h"

namespace ni {

class ComponentStore
{
private:
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<TransformComponent>> transform_components_;
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<PhysicsComponent>>   physics_components_;
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<UpdateComponent>>	   update_components_;
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<GraphicsComponent>>  graphics_components_;

public:
	ComponentStore() = default;

	void AttachPhysicsComponent(Id<GameObjectTag> target, PhysicsComponent& component)
	{		
		physics_components_.emplace(target, std::make_unique<PhysicsComponent>(component));
	}

	void AttachUpdateComponent(Id<GameObjectTag> target, std::unique_ptr<UpdateComponent> component)
	{
		update_components_.emplace(target, std::move(component));
	}

	void AttachGraphicsComponent(Id<GameObjectTag> target, std::unique_ptr<GraphicsComponent> component)
	{
		graphics_components_.emplace(target, std::move(component));
	}

	void AttachTransformComponent(Id<GameObjectTag> target, TransformComponent& component)
	{
		transform_components_.emplace(target, std::make_unique<TransformComponent>(component));
	}

	void RemovePhysicsComponent(Id<GameObjectTag> target)
	{
		physics_components_.erase(target);
	}

	void RemoveUpdateComponent(Id<GameObjectTag> target)
	{
		update_components_.erase(target);
	}

	void RemoveGraphicsComponent(Id<GameObjectTag> target)
	{
		graphics_components_.erase(target);
	}

	void RemoveTransformComponent(Id<GameObjectTag> target)
	{
		transform_components_.erase(target);
	}

	TransformComponent* GetTransformComponent(Id<GameObjectTag> id);

	void PhysicsUpdate();
	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}