#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/Camera.h>
#include <NiEngine/BitmapStore.h>

#include "EntityFactory.h"

class PlatformerGameMode : public ni::GameMode
{
public:
	PlatformerGameMode();

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	ni::Camera camera_;

	EntityFactory entity_factory_;
};

