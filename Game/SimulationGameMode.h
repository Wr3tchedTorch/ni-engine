#pragma once

#include <vector>
#include <unordered_map>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <BitmapStore.h>
#include <GameMode.h>
#include <GameModeController.h>
#include <Camera.h>

#include "Ball.h"
#include "BallFactory.h"

class SimulationGameMode : public ni::GameMode
{
private:
	BallFactory ball_factory_;
	ni::Camera  main_camera_;

	std::vector<Ball> balls_ = {};

	bool was_mouse_pressed_ = false;

	std::unordered_map<sf::Mouse::Button, bool> mouse_pressed_history_;

	sf::Vector2f current_mouse_position_;

public:
	SimulationGameMode();

	bool IsMouseButtonJustPressed(sf::Mouse::Button button);	

	virtual void Update(ni::GameModeController& controller) override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;
};

