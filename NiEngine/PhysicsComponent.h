#pragma once

#include "Component.h"
#include "GameObject.h"

namespace ni {

class PhysicsComponent : public Component
{
public:
	virtual void update(GameObject& parent) = 0;
};

}