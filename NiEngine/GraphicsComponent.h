#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Component.h"
#include "BitmapStore.h"

namespace ni {

class GraphicsComponent : public Component
{
protected:
	std::string texture_key_;
	sf::IntRect first_frame_rect_;

public:
	GraphicsComponent() = default;
	GraphicsComponent(std::string texture_key, sf::IntRect frame_rect = {});

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}