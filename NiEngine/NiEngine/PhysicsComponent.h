#pragma once

#include <id.h>

#include <NiEngine/Component.h>

namespace ni {

class TransformComponent;

class PhysicsComponent : public Component
{

public:
	virtual void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id) = 0;
};

}