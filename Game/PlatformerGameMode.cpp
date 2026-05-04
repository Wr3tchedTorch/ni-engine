#include "PlatformerGameMode.h"

#include <types.h>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/WipeScreenTransition.h>
#include <NiEngine/GameModeController.h>

#include "PlatformerObjectFactory.h"

PlatformerGameMode::PlatformerGameMode()
{
	ni::Converter::pixels_per_meters_ = 16;

	b2WorldDef world_def = b2DefaultWorldDef();
	world_def.gravity = { 0.0f, 9.8f };
	GetPhysicsEngine().CreateWorld(world_def);
	
	auto factory = std::make_unique<PlatformerObjectFactory>();
	level_.RegisterObjectFactory(std::move(factory));

	level_.LoadNextLevel(*this);
	camera_.FitTo(level_.GetCurrentTilemap().GetBounds());
	
	current_transition_ = std::make_unique<ni::WipeScreenTransition>(.4f, camera_.GetView().getSize(), false, sf::Color::Black);
	current_transition_->Play(true);

	current_transition_->OnTransitionCoveredScreen([this]() {
		if (restart_level_)
		{
			level_.ReloadLevel(*this);
			restart_level_ = false;
		}
		else if (load_next_level_)
		{
			level_.LoadNextLevel(*this);
			load_next_level_ = false;
		}
	});

	current_transition_->OnTransitionFinished([this]() {
		transitioning_ = false;
		});
}

void PlatformerGameMode::PrepareToLoadNextLevel()
{
	load_next_level_ = true;
}

void PlatformerGameMode::RestartLevel()
{
	restart_level_ = true;
}

void PlatformerGameMode::Update(ni::GameModeController& controller)
{
	if ((restart_level_ || load_next_level_) && !transitioning_)
	{
		current_transition_->Play();
		transitioning_ = true;
		return;
	}
	GameMode::Update(controller);
}

void PlatformerGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	camera_.ApplyTo(target);

	ni::GameMode::Render(target, states, store);
}
