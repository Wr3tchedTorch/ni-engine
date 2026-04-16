#pragma once

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/PlatformerCharacterPhysicsComponent.h>
#include <NiEngine/ComponentLocator.h>

class PlayerUpdateComponent : public ni::UpdateComponent
{
private:
	ni::PlatformerCharacterPhysicsComponent* physics_component_;

public:
	PlayerUpdateComponent(ni::Id<GameObjectTag> owner_id, ni::ComponentLocator& component_locator);

	void Update() override;
};

