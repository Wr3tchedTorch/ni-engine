#pragma once

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameObjectTag.h"
#include "Id.h"

namespace ni {

class ComponentLocator
{
public:
	virtual TransformComponent* GetTransformComponent(Id<GameObjectTag> id) = 0;
	virtual PhysicsComponent*   GetPhysicsComponent(Id<GameObjectTag> id)   = 0;
};

}