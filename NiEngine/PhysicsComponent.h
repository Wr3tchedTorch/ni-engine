#pragma once

#include <id.h>

#include "Component.h"
#include "TransformComponent.h"

namespace ni {

class PhysicsComponent : public Component
{
private:
	b2BodyId body_;

public:
	PhysicsComponent(b2BodyId body);
	
	b2BodyId GetBodyId() const
	{
		return body_;
	}

	virtual void PhysicsUpdate(TransformComponent& transform_component);
};

}