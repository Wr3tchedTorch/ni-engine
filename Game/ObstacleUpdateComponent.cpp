#include "ObstacleUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>

ObstacleUpdateComponent::ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> player_id) : ni::UpdateComponent(component_locator), 
	player_id_(player_id)
{}

void ObstacleUpdateComponent::Update()
{

}
