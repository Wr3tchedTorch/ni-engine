#include "Engine.h"

#include <optional>
#include <string>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

ni::Engine::Engine(std::string window_name, sf::State start_state)
{
	window_.create(sf::VideoMode::getDesktopMode(), window_name, start_state);
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

		window_.clear(sf::Color::Blue);

		window_.display();
	}
}
