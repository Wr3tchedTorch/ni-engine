#include "TransformComponent.h"

#include <math_functions.h>

#include <SFML/System/Angle.hpp>

#include "Converter.h"

void ni::TransformComponent::setPositionInMeters(b2Vec2 position)
{
	transformable_.setPosition(converter::metersToPixels(position));
}

b2Vec2 ni::TransformComponent::getPositionInMeters() const
{
	return converter::pixelsToMeters(transformable_.getPosition());
}

void ni::TransformComponent::setRotation(float radians)
{
	transformable_.setRotation(sf::radians(radians));
}

float ni::TransformComponent::getRotationInRadians() const
{
	return transformable_.getRotation().asRadians();
}
