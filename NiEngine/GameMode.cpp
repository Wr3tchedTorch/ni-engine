#include "GameMode.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"

void ni::GameMode::update()
{
	store_.update();

	store_.physicsUpdate();
}

void ni::GameMode::render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	store_.render(target, states, store);
}
