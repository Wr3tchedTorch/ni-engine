#include <NiEngine/SoundEngine.h>

#include <string>
#include <unordered_set>

#include <SFML/Audio/Sound.hpp>
#include <NiEngine/SoundBufferStore.h>

void ni::SoundEngine::Preload(const std::string& key)
{
	sounds_map_.emplace(key, new sf::Sound(sound_buffer_store_.GetSoundBuffer(key)));
}

void ni::SoundEngine::PlaySound(std::string key)
{
	sound_queue_.push(key);
}

void ni::SoundEngine::FlushSoundQueue()
{
	std::unordered_set<std::string> played_keys;
	while (!sound_queue_.empty())
	{
		std::string key = sound_queue_.front();
		sound_queue_.pop();
		if (played_keys.contains(key))
		{
			continue;
		}
		played_keys.insert(key);

		auto it = sounds_map_.find(key);
		if (it != sounds_map_.end())
		{
			it->second->stop();
			it->second->play();
		}
	}
}
