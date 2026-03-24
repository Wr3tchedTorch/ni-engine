#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Component.h"
#include "GameObject.h"

namespace ni {

class GraphicsComponent : public Component, public sf::Drawable
{
public:
	virtual void update(GameObject& parent) = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

}