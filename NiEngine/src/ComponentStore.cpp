#include <NiEngine/ComponentStore.h>

#include <id.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/PhysicsComponent.h>

ni::TransformComponent* ni::ComponentStore::GetTransformComponent(Id<GameObjectTag> id)
{
	auto it = transform_components_.find(id);

	if (it == transform_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

ni::PhysicsComponent* ni::ComponentStore::GetPhysicsComponent(Id<GameObjectTag> id)
{
	auto it = physics_components_.find(id);

	if (it == physics_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

void ni::ComponentStore::PhysicsUpdate(b2WorldId world_id)
{
	for (auto& [id, component] : physics_components_)
	{
		TransformComponent* transform = GetTransformComponent(id);

		if (!transform)
		{
			continue;
		}
		component->PhysicsUpdate(*transform, world_id);
	}
}

void ni::ComponentStore::Update()
{
	for (auto& [id, component] : update_components_)
	{
		component->Update();
	}
}

void ni::ComponentStore::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	for (auto& [id, components] : graphics_components_)
	{
		sf::RenderStates local_state = states;

		TransformComponent* transform = GetTransformComponent(id);
		if (transform)
		{
			local_state.transform *= transform->GetTransformable().getTransform();
		}		

		for (auto& component : components)
		{
			component->Render(target, local_state, store);
		}

	}
}