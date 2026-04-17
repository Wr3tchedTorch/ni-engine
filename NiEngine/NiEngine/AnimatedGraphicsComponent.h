#pragma once

#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

#include "StandardGraphicsComponent.h"
#include "BitmapStore.h"

namespace ni {

class AnimatedGraphicsComponent : public StandardGraphicsComponent
{
public:
	AnimatedGraphicsComponent(std::string texture_key, sf::IntRect first_frame_rect, int frame_count, int animation_count, int frame_spacing = 0);

	void PlayAnimation(int animation_row, float delay_in_seconds, bool loop = false);
	bool IsPlaying() const;

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	sf::IntRect initial_frame_rect_;

	int frame_count_	 = 0;
	int animation_count_ = 0;
	int frame_spacing_   = 0;

	int current_animation_ = -1;
	int current_frame_	   = 0;

	bool loop_    = false;
	bool playing_ = false;

	float delay_in_seconds_ = 0;
	sf::Time time_since_last_animation_frame_;

	void NextFrame();
};

}