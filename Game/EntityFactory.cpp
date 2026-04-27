#include "EntityFactory.h"

#include <memory>
#include <utility>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/StandardGraphicsComponent.h>
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
	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>("graphics/tilemap.png", sf::Vector2i(16, 16), 1);	
	auto update   = std::make_unique<PlayerUpdateComponent>(game_mode.GetComponentStore(), id);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition({100, 100});
	transform.GetTransformable().setOrigin({ character_size.x / 2.0f, character_size.y / 2.0f });	

	update->Init(*graphics.get(), *physics.get());
	
	game_mode.GetComponentStore().AttachPhysicsComponent  (id, std::move(physics));
	game_mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	game_mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	game_mode.GetComponentStore().AttachTransformComponent(id, transform);

	return id;
}

ni::Id<ni::GameObjectTag> EntityFactory::CreateObstacle(ni::GameMode& game_mode, sf::Vector2i obstacle_size, sf::Vector2i texture_coords, bool collidable, bool harmful)
{
	ni::Id<ni::GameObjectTag> id = game_mode.CreateGameObject();

	sf::IntRect frame_rect;
	frame_rect.position.x = texture_coords.x * obstacle_size.x;
	frame_rect.position.y = texture_coords.y * obstacle_size.y;
	frame_rect.size = obstacle_size;

	auto graphics = std::make_unique<ni::StandardGraphicsComponent>("graphics/tilemap.png", frame_rect);
	
	ni::TransformComponent transform;
	transform.GetTransformable().setPosition({ 100, 100 });

	game_mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	game_mode.GetComponentStore().AttachTransformComponent(id, transform);

	return id;
}
