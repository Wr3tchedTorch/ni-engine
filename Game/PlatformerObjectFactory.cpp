#include "PlatformerObjectFactory.h"

#include <vector>
#include <string>
#include <utility>
#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/TiledUtility.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/StandardGraphicsComponent.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "ObstacleHarmfulCollisionComponent.h"
#include "MovingObstacleUpdateComponent.h"
#include "PlatformerGameMode.h"
#include "CharacterPhysicsComponent.h"
#include "PlayerUpdateComponent.h"
#include "ExitDoorUpdateComponent.h"

void PlatformerObjectFactory::SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type)
{
	ni::TilesetBlueprint tileset = ni::TiledUtility::GetTilesetByGid(tileset_blueprints, object_template.tile_gid_);
	std::string  texture_key = tileset.texture_key_;
	
	int gid  = object_template.tile_gid_ - tileset.first_gid_;

	int uv_x = gid % tileset.columns_;
	int uv_y = gid / tileset.columns_;
	
	sf::IntRect texture_coords;
	texture_coords.position.x = uv_x * (tileset.tile_size_.x + tileset.spacing_);
	texture_coords.position.y = uv_y * (tileset.tile_size_.y + tileset.spacing_);
	texture_coords.size = tileset.tile_size_;

	switch (type)
	{
	case ObjectTypes::Spike:
		SpawnSpike(object, object_template, texture_key, texture_coords, mode);
		break;
	case ObjectTypes::Player:
		SpawnPlayer(object, object_template, texture_key, texture_coords, mode);
		break;
	case ObjectTypes::ExitDoor:
		SpawnExitDoor(object, object_template, texture_key, texture_coords, mode);
	}
}

void PlatformerObjectFactory::SpawnPlayer(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	mode.GetComponentStore().RegisterTagForId(id, PlatformerGameMode::kPlayerTag);

	auto physics  = std::make_unique<CharacterPhysicsComponent>(texture_coordinates.size);
	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);

	auto& platformer_mode = static_cast<PlatformerGameMode&>(mode);
	auto update   = std::make_unique<PlayerUpdateComponent>(mode.GetComponentStore(), id, platformer_mode);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({ texture_coordinates.size.x / 2.0f, texture_coordinates.size.y / 2.0f });

	update->Init(*graphics.get(), *physics.get());

	mode.GetComponentStore().RegisterTagForId(id, PlatformerGameMode::kPlayerTag);
	mode.GetComponentStore().AttachPhysicsComponent(id, std::move(physics));
	mode.GetComponentStore().AttachUpdateComponent(id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent(id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnSpike(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	sf::Vector2f movement_offset = {};
	movement_offset.y    = object_template.properties_map_.at("movement_offset_y").GetValue<float>();
	movement_offset.x    = object_template.properties_map_.at("movement_offset_x").GetValue<float>();
	float movement_delay = object_template.properties_map_.at("movement_delay")   .GetValue<float>();

	sf::Vector2i repeat;
	repeat.y = object_template.properties_map_.at("repeat_y").GetValue<int>();
	repeat.x = object_template.properties_map_.at("repeat_x").GetValue<int>();

	if (object.properties_.contains("movement_offset_y")) movement_offset.y = object.properties_.at("movement_offset_y").GetValue<float>();
	if (object.properties_.contains("movement_offset_x")) movement_offset.x = object.properties_.at("movement_offset_x").GetValue<float>();
	if (object.properties_.contains("repeat_y"         )) repeat.y = object.properties_.at("repeat_y").GetValue<int>();
	if (object.properties_.contains("repeat_x"         )) repeat.x = object.properties_.at("repeat_x").GetValue<int>();
	if (object.properties_.contains("movement_delay"   )) movement_delay    = object.properties_.at("movement_delay"   ).GetValue<float>();

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	mode.GetComponentStore().RegisterTagForId(id, PlatformerGameMode::kSpikeTag);

	auto graphics = std::make_unique<ni::StandardGraphicsComponent>(texture_key, texture_coordinates);
	graphics->SetRepeating(repeat);

	sf::Vector2f object_top_left_position = object.position_;
	object_top_left_position.y -= texture_coordinates.size.y;	

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object_top_left_position);

	sf::Vector2f collision_size;
	collision_size.x = texture_coordinates.size.x + texture_coordinates.size.x * repeat.x;
	collision_size.y = texture_coordinates.size.y + texture_coordinates.size.y * repeat.y;

	auto update = std::make_unique<MovingObstacleUpdateComponent>(
		mode.GetComponentStore(), transform, id,
		sf::Vector2i(movement_offset), 20,
		collision_size,
		movement_delay
	);
	update->RegisterCollisionComponent(std::make_unique<ObstacleHarmfulCollisionComponent>());

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnExitDoor(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);
	graphics->SetFrame(2, 16);

	auto update = std::make_unique<ExitDoorUpdateComponent>(mode, mode.GetComponentStore(), id, sf::Vector2f(texture_coordinates.size));

	sf::Vector2f object_top_left_position = object.position_;
	object_top_left_position.y -= texture_coordinates.size.y;

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object_top_left_position);

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
