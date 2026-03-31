#pragma once

#include <math_functions.h>

#include <SFML/Graphics/Transformable.hpp>

#include "Component.h"

namespace ni {

class TransformComponent : public Component
{
private:
	sf::Transformable transformable_;

public:
	sf::Transformable& getTransformable()
	{
		return transformable_;
	}

	void setPositionInMeters(b2Vec2 position);
	b2Vec2 getPositionInMeters() const;

	void  setRotation(float radians);
	float getRotationInRadians() const;
};

}