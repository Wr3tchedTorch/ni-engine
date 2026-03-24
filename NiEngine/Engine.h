#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowEnums.hpp>

namespace ni {

class Engine
{
private:
	sf::RenderWindow window_;

public:
	Engine(std::string window_name, sf::State start_state);
	void Run();
};

}