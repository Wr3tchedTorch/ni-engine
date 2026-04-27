#pragma once

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ComponentLocator.h>

class ObstacleUpdateComponent : public ni::UpdateComponent
{
public:
	ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, ni::Id<ni::GameObjectTag> player_id);
	
	void Update() override;

protected:
	ni::Id<ni::GameObjectTag> player_id_;
	
	bool harmful_   = false;
	bool collidable = false;

private:
	void HandleCollisions();
};

