#include "TransformComponent.h"

#include <math_functions.h>

#include <SFML/System/Vector2.hpp>

#include "Converter.h"

void ni::TransformComponent::setPosition(sf::Vector2f position)
{
	transform_.setPosition(position);
}

void ni::TransformComponent::setPositionInMeters(b2Vec2 position)
{
	transform_.setPosition(converter::metersToPixels(position));
}

sf::Vector2f ni::TransformComponent::getPosition() const
{
	return transform_.getPosition();
}

b2Vec2 ni::TransformComponent::getPositionInMeters() const
{
	return converter::pixelsToMeters(transform_.getPosition());
}

void ni::TransformComponent::setRotation(float radians)
{
	transform_.setRotation(sf::radians(radians));
}

float ni::TransformComponent::getRotation() const
{
	return transform_.getRotation().asRadians();
}

void ni::TransformComponent::setScale(sf::Vector2f scale)
{
	transform_.setScale(scale);
}

sf::Vector2f ni::TransformComponent::getScale() const
{
	return transform_.getScale();
}

void ni::TransformComponent::setOrigin(sf::Vector2f origin)
{
	transform_.setOrigin(origin);
}

sf::Vector2f ni::TransformComponent::getOrigin()
{
	return transform_.getOrigin();
}
