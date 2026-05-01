#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/Camera.h>
#include <NiEngine/BitmapStore.h>

class PlatformerGameMode : public ni::GameMode
{
public:
	inline static const std::string kSpikeTag  = "spike";
	inline static const std::string kPlayerTag = "player";

	PlatformerGameMode();

	void RestartLevel();
	void PrepareToLoadNextLevel();
	virtual void Update(ni::GameModeController& controller) override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	ni::Camera camera_;

	bool restart_level_   = false;
	bool load_next_level_ = false;
};

