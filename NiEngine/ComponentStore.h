#pragma once

#include <unordered_map>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "GameObjectId.h"
#include "GraphicsComponent.h"
#include "UpdateComponent.h"
#include "PhysicsComponent.h"
#include "BitmapStore.h"

namespace ni {

class ComponentStore
{
private:
	std::unordered_map<GameObjectId, PhysicsComponent>  physics_component_;
	std::unordered_map<GameObjectId, UpdateComponent>	update_component_;
	std::unordered_map<GameObjectId, GraphicsComponent> graphics_component_;

public:

	void addPhysicsComponent(GameObjectId target, PhysicsComponent component)
	{
		physics_component_[target] = component;
	}

	void addUpdateComponent(GameObjectId target, UpdateComponent component)
	{
		update_component_[target] = component;
	}

	void addGraphicsComponent(GameObjectId target, GraphicsComponent component)
	{
		graphics_component_[target] = component;
	}

	void removePhysicsComponent(GameObjectId target)
	{
		physics_component_.erase(target);
	}

	void removeUpdateComponent(GameObjectId target)
	{
		update_component_.erase(target);
	}

	void removeaddGraphicsComponent(GameObjectId target)
	{
		graphics_component_.erase(target);
	}

	void update();
	void render(sf::RenderTarget& target, sf::RenderStates& states, BitmapStore& store);
};

}