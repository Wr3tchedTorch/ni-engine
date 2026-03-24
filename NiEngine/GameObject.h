#pragma once

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

namespace ni {

class GameObject : public sf::Drawable, public sf::Transformable
{
private:
	std::unique_ptr<InputComponent>	   m_Input;
	std::unique_ptr<PhysicsComponent>  m_Physics;
	std::unique_ptr<GraphicsComponent> m_Graphics;

public:
	void update();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}