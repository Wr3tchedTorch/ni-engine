#pragma once

#include <string>
#include <queue>
#include <unordered_map>

#include <SFML/Audio/Sound.hpp>

#include "SoundBufferStore.h"

namespace ni {

class SoundEngine
{

friend class Engine;

public:
	void Preload(const std::string& key);
	void PlaySound(std::string key);
	void FlushSoundQueue();

private:
	inline static const int kMaxSoundsPerFrame = 16;

	SoundBufferStore sound_buffer_store_;

	std::queue<std::string> sound_queue_;
	std::unordered_map<std::string, sf::Sound*> sounds_map_;
};

}