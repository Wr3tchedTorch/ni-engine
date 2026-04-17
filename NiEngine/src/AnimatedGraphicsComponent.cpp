#include <NiEngine/AnimatedGraphicsComponent.h>

#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/StandardGraphicsComponent.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Engine.h>

ni::AnimatedGraphicsComponent::AnimatedGraphicsComponent(std::string texture_key, sf::IntRect first_frame_rect, int frame_count, int animation_count, int frame_spacing) :
	ni::StandardGraphicsComponent(texture_key, first_frame_rect)
{
	initial_frame_rect_ = first_frame_rect;

	frame_count_	 = frame_count;
	animation_count_ = animation_count;

	frame_spacing_ = frame_spacing;
}

void ni::AnimatedGraphicsComponent::PlayAnimation(int animation_row, float delay_in_seconds, bool loop)
{
	if (current_animation_ == animation_row)
	{
		return;
	}
	current_animation_ = animation_row;
	current_frame_ = 0;

	delay_in_seconds_ = delay_in_seconds;
	loop_    = loop;
	playing_ = true;
}

bool ni::AnimatedGraphicsComponent::IsPlaying() const
{
	return playing_;
}

void ni::AnimatedGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	float time_elapsed_since_last_frame = (ni::Engine::time_elapsed - time_since_last_animation_frame_).asSeconds();

	if (playing_ && time_elapsed_since_last_frame >= delay_in_seconds_)
	{
		NextFrame();
	}

	ni::StandardGraphicsComponent::Render(target, states, store);
}

void ni::AnimatedGraphicsComponent::NextFrame()
{
	current_frame_++;
	if (current_frame_ < frame_count_)
	{
		current_frame_rect_ = initial_frame_rect_;
		current_frame_rect_.position.x += (current_frame_rect_.size.x + frame_spacing_) * current_frame_;
		
		time_since_last_animation_frame_ = ni::Engine::time_elapsed;
		return;
	}
	if (loop_)
	{
		current_frame_ = -1;
		return;
	}
	playing_ = false;
}
