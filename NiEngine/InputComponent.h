#pragma once

#include "Component.h"
#include "GameObject.h"

namespace ni {

class InputComponent : public Component
{
public:	
	virtual void update(GameObject& parent) = 0;
};

}