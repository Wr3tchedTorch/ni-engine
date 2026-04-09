
#include <utility>
#include <memory>

#include <SFML/Window/WindowEnums.hpp>

#include <NiEngine/Id.h>
#include <NiEngine/Engine.h>
#include <NiEngine/GameModeTag.h>

#include "SimulationGameMode.h"

int main()
{
    ni::Engine engine("Ni Engine, by Eric", sf::State::Fullscreen);

    auto mode = std::make_unique<SimulationGameMode>();

    ni::Id<GameModeTag> mode_id = engine.GetGameModeController().Register(std::move(mode));
    engine.GetGameModeController().SwitchTo(mode_id);

    engine.Run();
}