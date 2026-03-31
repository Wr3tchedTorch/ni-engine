#include "GraphicsComponent.h"

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "BitmapStore.h"

ni::GraphicsComponent::GraphicsComponent(std::string texture_key) : texture_key_(texture_key)
{
}

void ni::GraphicsComponent::render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	sf::Sprite sprite(store.getTexture(texture_key_));

	target.draw(sprite, states);
}