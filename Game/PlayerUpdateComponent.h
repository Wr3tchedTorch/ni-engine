#pragma once

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class PlayerUpdateComponent : public ni::UpdateComponent
{
public:
	PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id);
	void Update() override;

private:
	void Jump();
};

