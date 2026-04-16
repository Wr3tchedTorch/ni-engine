#pragma once

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/Component.h>

namespace ni {

class UpdateComponent : public Component
{
protected:
	ComponentLocator& component_locator_;

public:
	UpdateComponent(ComponentLocator& component_locator) : component_locator_(component_locator)
	{
	}

	virtual void Update() = 0;
};

}