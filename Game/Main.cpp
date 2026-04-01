

#include <SFML/Window/WindowEnums.hpp>
#include <Engine.h>
#include <GameMode.h>
#include <GameObjectTag.h>
#include <GraphicsComponent.h>
#include <TransformComponent.h>
#include <Id.h>
#include <GameModeTag.h>
#include <GameModeController.h>
#include <memory>
#include <utility>

int main()
{
    ni::Engine engine("Shooter by Eric", sf::State::Fullscreen);
    ni::GameModeController& controller = engine.getGameModeController();

    auto my_mode = std::make_unique<ni::GameMode>();

    ni::Id<GameObjectTag>  player_id = my_mode->CreateGameObject();
    ni::GraphicsComponent  player_graphics("graphics/player.png");
    ni::TransformComponent player_transform;
    player_transform.SetPositionInMeters({ 5, 5 });
    player_transform.GetTransformable().setScale({ .1f, .1f });

    ni::Id<GameModeTag> mode_id = controller.Register(std::move(my_mode));
    controller.SwitchTo(mode_id);

    controller.GetCurrent().GetComponentStore().AttachGraphicsComponent(player_id, player_graphics);
    controller.GetCurrent().GetComponentStore().AttachTransformComponent(player_id, player_transform);

    auto my_mode2 = std::make_unique<ni::GameMode>();
    player_transform.SetRotation(sf::degrees(45).asRadians());

    ni::Id<GameModeTag> mode2_id = controller.Register(std::move(my_mode2));
    controller.SwitchTo(mode2_id);

    controller.GetCurrent().GetComponentStore().AttachGraphicsComponent(player_id, player_graphics);
    controller.GetCurrent().GetComponentStore().AttachTransformComponent(player_id, player_transform);

    //engine.getGameModeController().Register(std::move(my_mode2));

    engine.Run();
}