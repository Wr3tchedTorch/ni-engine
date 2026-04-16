#pragma once

#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>

namespace ni {
	class PlatformerCharacterPhysicsComponent;
}

class PlayerUpdateComponent : public ni::UpdateComponent
{
private:
	ni::PlatformerCharacterPhysicsComponent* physics_component_;

public:
	PlayerUpdateComponent(ni::Id<GameObjectTag> owner_id, ni::ComponentLocator& component_locator);

	void Update() override;
};

