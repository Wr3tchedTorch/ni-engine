#include "Engine.h"

#include <optional>
#include <string>
#include <utility>
#include <memory>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "GameMode.h"

ni::Engine::Engine(std::string window_name, sf::State start_state)
{
	window_.create(sf::VideoMode::getDesktopMode(), window_name, start_state);
}

void ni::Engine::registerGameMode(std::unique_ptr<GameMode> mode)
{
	current_game_mode_ = std::move(mode);
}

ni::GameMode* ni::Engine::getGameMode()
{
	return current_game_mode_.get();
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

		current_game_mode_->update();

		window_.clear(sf::Color::Black);

		current_game_mode_->render(window_, sf::RenderStates::Default, store_);

		window_.display();
	}
}
