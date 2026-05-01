#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine\ObjectFactory.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>

enum ObjectTypes
{
	None     = 0,
	Spike    = 1,
	Player   = 2,
	ExitDoor = 3
};

class PlatformerObjectFactory : public ni::ObjectFactory
{
private:
	void SpawnObject  (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type) override;
	void SpawnPlayer  (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
	void SpawnSpike   (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
	void SpawnExitDoor(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
};

