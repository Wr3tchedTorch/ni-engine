#pragma once

#include <vector>

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameObjectTag.h"
#include "Id.h"
#include "GraphicsComponent.h"
#include "AnimatedGraphicsComponent.h"

namespace ni {

class ComponentLocator
{
public:
	virtual TransformComponent* GetTransformComponent(Id<GameObjectTag> id) = 0;
	virtual PhysicsComponent*   GetPhysicsComponent(Id<GameObjectTag> id)   = 0;
	virtual std::vector<GraphicsComponent*> GetGraphicsComponents(Id<GameObjectTag> id) = 0;
	virtual AnimatedGraphicsComponent* GetFirstAnimatedGraphicsComponent(Id<GameObjectTag> id) = 0;
};

}