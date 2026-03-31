#include "ComponentStore.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"

void ni::ComponentStore::update()
{
	for (auto& [id, component] : graphics_component_)
	{
		component.update();
	}
}

void ni::ComponentStore::render(sf::RenderTarget& target, sf::RenderStates& states, BitmapStore& store)
{
	for (auto& [id, component] : graphics_component_)
	{
		component.render(target, states, store);
	}
}
