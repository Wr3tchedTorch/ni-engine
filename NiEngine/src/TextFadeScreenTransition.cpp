#include <NiEngine/TextFadeScreenTransition.h>

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/ScreenTransition.h>
#include <NiEngine/Engine.h>
#include <NiEngine/BitmapStore.h>

ni::TextFadeScreenTransition::TextFadeScreenTransition(
	float delay_in_seconds, 
	const std::string& text_string,
	const std::string& font_path,
	int font_size,
	sf::Color text_color, 
	sf::Color background_color, 
	sf::Vector2f camera_size) : ScreenTransition(delay_in_seconds), main_font_(font_path), main_text_(main_font_, text_string, font_size)
{	
	main_text_.setFillColor(text_color);

	sf::Vector2f main_text_position = camera_size;
	main_text_position -= camera_size / 2.0f;
	main_text_position.x -= font_size / 2.0f;
	main_text_position.y -= font_size / 2.0f;
	main_text_.setPosition(main_text_position);	
}

void ni::TextFadeScreenTransition::Update()
{
	float time_elapsed = (Engine::time_elapsed - time_since_start_).asSeconds();
	if (time_elapsed < delay_in_seconds_)
	{
		LerpTextOpacity(time_elapsed);
		return;
	}
	if (!playing_reversed_)
	{
		LerpTextOpacity(1);
		playing_reversed_ = true;
		time_since_start_ = Engine::time_elapsed;
		on_transition_covered_screen_.Notify();
		return;
	}
	on_transition_finished_.Notify();
	playing_ = false;
	playing_reversed_ = false;
}

void ni::TextFadeScreenTransition::Render(sf::RenderTarget & target, sf::RenderStates states, BitmapStore & store)
{
}

void ni::TextFadeScreenTransition::LerpTextOpacity(float time_elapsed)
{
	
}

