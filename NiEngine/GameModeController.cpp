#include "GameModeController.h"

#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "BitmapStore.h"
#include "GameMode.h"
#include "GameModeTag.h"
#include "Id.h"

ni::GameModeController::GameModeController()
{
	current_id = { 0 };
	next_id    = { 0 };
}

ni::Id<GameModeTag> ni::GameModeController::Register(std::unique_ptr<GameMode> mode)
{
	game_modes_.emplace(++next_id, std::move(mode));
	return next_id;
}

void ni::GameModeController::SwitchTo(Id<GameModeTag> id)
{
	current_id = id;
}

void ni::GameModeController::Remove(Id<GameModeTag> id)
{
	game_modes_.erase(id);
}

ni::GameMode& ni::GameModeController::Get(Id<GameModeTag> id)
{
	return *game_modes_.at(id).get();
}

ni::GameMode& ni::GameModeController::GetCurrent()
{
	return *game_modes_.at(current_id).get();
}

void ni::GameModeController::Update()
{	
	if (current_id.id_ == 0)
	{
		return;
	}
	game_modes_.at(current_id)->Update();
}

void ni::GameModeController::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	if (current_id.id_ == 0)
	{
		return;
	}
	game_modes_.at(current_id)->Render(target, states, store);
}
