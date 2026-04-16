#pragma once

#include <NiEngine/Component.h>
#include <NiEngine/TransformComponent.h>

namespace ni {

class PhysicsComponent : public Component
{
public:
	virtual void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id) = 0;
};

}