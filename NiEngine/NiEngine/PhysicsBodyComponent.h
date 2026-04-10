#pragma once

#include <id.h>

#include "PhysicsComponent.h"
#include "TransformComponent.h"

namespace ni {

class PhysicsBodyComponent : public PhysicsComponent
{
private:
	b2BodyId body_id_;

public:
	PhysicsBodyComponent(b2BodyId body_id);

	b2BodyId GetBodyId() const { return body_id_; };

	void PhysicsUpdate(TransformComponent& transform_component) override;
};

}