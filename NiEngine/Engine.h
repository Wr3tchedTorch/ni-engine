#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include "BitmapStore.h"
#include "GameModeController.h"

namespace ni {

class Engine
{

private:
	sf::RenderWindow window_;

	GameModeController game_mode_controller_;
	BitmapStore store_;

public:
	Engine(std::string window_name, sf::State start_state);

	GameModeController& getGameModeController();

	void Run();
};

}