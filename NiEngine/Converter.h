#pragma once

#include <math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace converter
{
	constexpr double PIXELS_PER_METERS = 32.0;
	constexpr double PI = 3.14159265358979323846;

	template<typename T>
	constexpr T pixelsToMeters(const T& x) { return x / PIXELS_PER_METERS; };

	template<typename T>
	constexpr T metersToPixels(const T& x) { return x * PIXELS_PER_METERS; };

	template<typename T>
	constexpr T degToRad(const T& x) { return PI * x / 180.0; };

	template<typename T>
	constexpr T radToDeg(const T& x) { return 180.0 * x / PI; }

	inline sf::Vector2f metersToPixels(const b2Vec2& v) 
	{
		return sf::Vector2f(v.x * PIXELS_PER_METERS, v.y * PIXELS_PER_METERS);
	}

	inline b2Vec2 pixelsToMeters(const sf::Vector2f& v)
	{
		return b2Vec2(v.x / PIXELS_PER_METERS, v.y / PIXELS_PER_METERS);
	}

	inline sf::Angle rotToAngle(const b2Rot& v)
	{
		return sf::Angle(sf::radians(b2Rot_GetAngle(v)));
	}
}
