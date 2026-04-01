#include "Engine.h"

#include <optional>
#include <string>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "GameModeController.h"

ni::Engine::Engine(std::string window_name, sf::State start_state)
{
	window_.create(sf::VideoMode::getDesktopMode(), window_name, start_state);
}

ni::GameModeController& ni::Engine::getGameModeController()
{
	return game_mode_controller_;
}

void ni::Engine::Run()
{
	while (window_.isOpen())
	{
		while (std::optional<sf::Event> event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window_.close();
			}
		}

		game_mode_controller_.Update();

		window_.clear(sf::Color::Black);

		game_mode_controller_.Render(window_, sf::RenderStates::Default, store_);

		window_.display();
	}
}
