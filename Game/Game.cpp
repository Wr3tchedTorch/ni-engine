
#include <GameMode.h>
#include <GameObjectId.h>

#include <memory>
#include <utility>

#include <SFML/Window/WindowEnums.hpp>
#include <GraphicsComponent.h>
#include <TransformComponent.h>
#include <Engine.h>;

int main()
{
    ni::Engine engine("Shooter by Eric", sf::State::Fullscreen);

    auto my_mode = std::make_unique<ni::GameMode>();
    engine.registerGameMode(std::move(my_mode));

    GameObjectId player_id = engine.getGameMode()->getNextGameObjectId();
   
    engine.getGameMode()->getComponentStore().attachGraphicsComponent(
        player_id, 
        std::make_unique<ni::GraphicsComponent>("graphics/player.png")
    );

    ni::TransformComponent player_transform;
    player_transform.setPositionInMeters({2, 2});
    player_transform.getTransformable().setScale({ 0.1f, 0.1f });

    engine.getGameMode()->getComponentStore().attachTransformComponent(
        player_id,
        std::make_unique<ni::TransformComponent>(player_transform)
    );

    engine.Run();
}