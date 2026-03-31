#pragma once

#include <math_functions.h>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Component.h"

namespace ni {

class TransformComponent : public Component
{
private:
	sf::Transformable transform_;

public:
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;

	void setPositionInMeters(b2Vec2 position);
	b2Vec2 getPositionInMeters() const;

	void  setRotation(float radians);
	float getRotation() const;

	void setScale(sf::Vector2f scale);
	sf::Vector2f getScale() const;

	void setOrigin(sf::Vector2f origin);
	sf::Vector2f getOrigin();
};

}