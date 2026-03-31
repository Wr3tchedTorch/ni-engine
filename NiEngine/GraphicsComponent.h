#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Component.h"
#include "BitmapStore.h"

namespace ni {

class GraphicsComponent : public Component
{
protected:
	std::string texture_key_;

public:
	GraphicsComponent() = default;
	GraphicsComponent(std::string texture_key);

	virtual void render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}