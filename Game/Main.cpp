

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

    sf::IntRect player_frame_rect = {
        {16 * 0, 16 * 15},
        {16, 16}
    };

    ni::GraphicsComponent  player_graphics("graphics/tilemap.png", player_frame_rect);   

    ni::TransformComponent player_transform;
    player_transform.GetTransformable().setPosition({ 200, 200 });

    ni::Id<GameModeTag> mode_id = controller.Register(std::move(my_mode));
    controller.SwitchTo(mode_id);

    controller.GetCurrent().GetComponentStore().AttachGraphicsComponent(player_id, player_graphics);
    controller.GetCurrent().GetComponentStore().AttachTransformComponent(player_id, player_transform);

    auto my_mode2 = std::make_unique<ni::GameMode>();
    player_transform.SetRotation(sf::degrees(45).asRadians());

    ni::Id<GameModeTag> mode2_id = controller.Register(std::move(my_mode2));
    controller.Get(mode2_id).GetComponentStore().AttachGraphicsComponent(player_id, player_graphics);
    controller.Get(mode2_id).GetComponentStore().AttachTransformComponent(player_id, player_transform);

    engine.Run();
}