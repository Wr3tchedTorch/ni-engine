#pragma once

#include "GameObjectId.h"

namespace ni {

class Component
{
protected:
	GameObjectId owner_id_;
};

}