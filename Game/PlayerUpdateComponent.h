#pragma once

#include <string>

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "CharacterPhysicsComponent.h"

class PlayerUpdateComponent : public ni::UpdateComponent
{
public:
	PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id);
	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics);
	void Update() override;

private:
	inline static const int kAnimationRow = 12;
	inline static const std::string kJumpAnimationKey = "jump";
	inline static const std::string kWalkAnimationKey = "walk";

	bool airborne_ = false;

	void Jump();
};

