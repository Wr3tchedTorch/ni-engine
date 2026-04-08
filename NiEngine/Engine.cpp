#include "Engine.h"

#include <optional>
#include <string>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "GameModeController.h"

sf::Time     ni::Engine::time_elapsed		= sf::Time();
sf::Vector2u ni::Engine::window_resolution  = {};

ni::Engine::Engine(std::string window_name, sf::State start_state)
{
	window_resolution = sf::VideoMode::getDesktopMode().size;

	window_.create(sf::VideoMode::getDesktopMode(), window_name, start_state);

	window_.setFramerateLimit(60);
}

ni::GameModeController& ni::Engine::GetGameModeController()
{
	return game_mode_controller_;
}

void ni::Engine::Run()
{
	sf::Clock deltaClock;
	while (window_.isOpen())
	{
		while (std::optional<sf::Event> event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window_.close();
			}
		}

		time_elapsed += deltaClock.restart();

		game_mode_controller_.Update();
		game_mode_controller_.PhysicsUpdate();

		window_.clear(sf::Color::Black);

		game_mode_controller_.Render(window_, sf::RenderStates::Default, store_);

		window_.display();
	}
}
