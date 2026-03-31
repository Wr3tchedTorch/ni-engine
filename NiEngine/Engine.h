#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include "GameMode.h"
#include "BitmapStore.h"

namespace ni {

class Engine
{

private:
	sf::RenderWindow window_;

	std::unique_ptr<GameMode> current_game_mode_;

	BitmapStore store_;

public:
	Engine(std::string window_name, sf::State start_state);

	void registerGameMode(std::unique_ptr<GameMode> mode);

	GameMode* getGameMode();

	void Run();
};

}