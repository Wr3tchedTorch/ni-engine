#pragma once

#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GraphicsComponent.h>

namespace ni {

class StandardGraphicsComponent : public GraphicsComponent
{
protected:
	std::string texture_key_;
	sf::IntRect current_frame_rect_;

public:
	StandardGraphicsComponent(std::string texture_key, sf::IntRect frame_rect = {});

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;
};

}