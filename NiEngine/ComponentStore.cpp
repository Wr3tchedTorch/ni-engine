#include "ComponentStore.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"
#include "GameObjectId.h"
#include "TransformComponent.h"

ni::TransformComponent* ni::ComponentStore::getTransformComponent(GameObjectId id)
{
	auto it = transform_components_.find(id);

	if (it == transform_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

void ni::ComponentStore::update()
{
	for (auto& [id, component] : update_components_)
	{
		component->update();
	}
}

void ni::ComponentStore::physicsUpdate()
{
	for (auto& [id, component] : physics_components_)
	{
		TransformComponent* transform = getTransformComponent(id);

		if (!transform)
		{
			continue;
		}
		component->physicsUpdate(*transform);
	}
}

void ni::ComponentStore::render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	for (auto& [id, component] : graphics_components_)
	{
		TransformComponent* transform = getTransformComponent(id);

		if (transform)
		{
			states.transform *= transform->getTransformable().getTransform();
		}
		component->render(target, states, store);
	}
}
