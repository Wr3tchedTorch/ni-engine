#pragma once

#include <cstdint>
#include <type_traits>

struct GameObjectId
{
	uint32_t id_ = 0;

	bool operator==(const GameObjectId& other) const
	{
		return id_ == other.id_;
	}

	GameObjectId& operator++()
	{
		id_++;
		return *this;
	}
};

template<>
struct std::hash<GameObjectId>
{
	size_t operator()(const GameObjectId& id) const
	{
		return std::hash<uint32_t>{}(id.id_);
	}
};