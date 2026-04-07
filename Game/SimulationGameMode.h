#pragma once

#include <vector>
#include <unordered_map>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Mouse.hpp>
#include <id.h>
#include <GameMode.h>
#include <GameModeController.h>

#include "Ball.h"
#include "BallFactory.h"

class SimulationGameMode : public ni::GameMode
{
private:
	BallFactory ball_factory_;

	std::vector<Ball> balls_ = {};

	bool was_mouse_pressed_ = false;

	std::unordered_map<sf::Mouse::Button, bool> mouse_pressed_history_;

public:
	SimulationGameMode();

	virtual void Update(ni::GameModeController& controller) override;
	bool IsMouseButtonJustPressed(sf::Mouse::Button button);	
};

