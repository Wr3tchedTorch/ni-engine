#include "EntityFactory.h"

#include <memory>
#include <utility>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "CharacterPhysicsComponent.h"
#include "PlayerUpdateComponent.h"

ni::Id<ni::GameObjectTag> EntityFactory::CreatePlatformerCharacter(ni::GameMode& game_mode, sf::Vector2i character_size, int animation_row)
{
	ni::Id<ni::GameObjectTag> id = game_mode.CreateGameObject();

	auto physics  = std::make_unique<CharacterPhysicsComponent>(character_size);
	auto update   = std::make_unique<PlayerUpdateComponent>(game_mode.GetComponentStore(), id);

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>("graphics/tilemap.png", sf::Vector2i(16, 16), 1);
	graphics->SetFrame(animation_row, 0);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition({100, 100});
	transform.GetTransformable().setOrigin({ character_size.x / 2.0f, character_size.x / 2.0f });
	
	game_mode.GetComponentStore().AttachPhysicsComponent(  id, std::move(physics));
	game_mode.GetComponentStore().AttachUpdateComponent(   id, std::move(update));
	game_mode.GetComponentStore().AttachGraphicsComponent( id, std::move(graphics));
	game_mode.GetComponentStore().AttachTransformComponent(id, transform);

	return id;
}
