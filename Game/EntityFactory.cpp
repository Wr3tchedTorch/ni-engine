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
#include "ObstacleHarmfulCollisionComponent.h"
#include "MovingObstacleUpdateComponent.h"

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

ni::Id<ni::GameObjectTag> EntityFactory::CreateObstacle(ni::GameMode& game_mode, ni::Id<ni::GameObjectTag> player_id, sf::Vector2i position, sf::IntRect texture_rect, bool collidable, bool harmful)
{
	ni::Id<ni::GameObjectTag> id = game_mode.CreateGameObject();

	sf::Vector2i repeat_amount = { 1, 0 };
	
	auto graphics = std::make_unique<ni::StandardGraphicsComponent>("graphics/tilemap.png", texture_rect);
	graphics->SetRepeating(repeat_amount);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(sf::Vector2f(position));

	auto update = std::make_unique<MovingObstacleUpdateComponent>(
		game_mode.GetComponentStore(), transform, id, player_id, 
		sf::Vector2i({ 0, -32 }), 20, 
		sf::Vector2f({ static_cast<float>((repeat_amount.x + 1) * 16), 16}),
		0.2f
	);
	update->RegisterCollisionComponent(std::make_unique<ObstacleHarmfulCollisionComponent>());

	game_mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	game_mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	game_mode.GetComponentStore().AttachTransformComponent(id, transform);

	return id;
}
