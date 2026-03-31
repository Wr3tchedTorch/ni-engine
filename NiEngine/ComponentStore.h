#pragma once

#include <unordered_map>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "GameObjectId.h"
#include "GraphicsComponent.h"
#include "UpdateComponent.h"
#include "PhysicsComponent.h"
#include "BitmapStore.h"
#include "TransformComponent.h"

namespace ni {

class ComponentStore
{
private:
	std::unordered_map<GameObjectId, std::unique_ptr<TransformComponent>> transform_components_;
	std::unordered_map<GameObjectId, std::unique_ptr<PhysicsComponent>>   physics_components_;
	std::unordered_map<GameObjectId, std::unique_ptr<UpdateComponent>>	  update_components_;
	std::unordered_map<GameObjectId, std::unique_ptr<GraphicsComponent>>  graphics_components_;

public:
	ComponentStore() = default;

	void attachPhysicsComponent(GameObjectId target, std::unique_ptr<PhysicsComponent>component)
	{
		physics_components_.emplace(target, std::move(component));
	}

	void attachUpdateComponent(GameObjectId target, std::unique_ptr<UpdateComponent> component)
	{
		update_components_.emplace(target, std::move(component));
	}

	void attachGraphicsComponent(GameObjectId target, std::unique_ptr<GraphicsComponent> component)
	{
		graphics_components_.emplace(target, std::move(component));
	}

	void attachTransformComponent(GameObjectId target, std::unique_ptr<TransformComponent> component)
	{
		transform_components_.emplace(target, std::move(component));
	}

	void removePhysicsComponent(GameObjectId target)
	{
		physics_components_.erase(target);
	}

	void removeUpdateComponent(GameObjectId target)
	{
		update_components_.erase(target);
	}

	void removeGraphicsComponent(GameObjectId target)
	{
		graphics_components_.erase(target);
	}

	void removeTransformComponent(GameObjectId target)
	{
		transform_components_.erase(target);
	}

	TransformComponent* getTransformComponent(GameObjectId id);

	void update();
	void physicsUpdate();
	void render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}