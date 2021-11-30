#include "PCH.h"
#include "AudioManager.h"

#include "core/Asserts.h"

#include "Data.h"


std::unordered_map<std::string, sf::SoundBuffer> AudioManager::s_buffers;
std::unordered_map<std::string, sf::Music> AudioManager::s_effects;

std::vector<std::string> AudioManager::s_nameCache;

float AudioManager::s_gvol = 100.f;
float AudioManager::s_mvol = 100.f;
float AudioManager::s_evol = 100.f;
float AudioManager::s_svol = 100.f;

sf::Music AudioManager::track;
std::string AudioManager::trackTitle;


sf::SoundBuffer& AudioManager::addSound(const std::string& name, const std::string& path)
{
	MNG_ASSERT_BASIC(s_buffers[name].loadFromFile(path));
	return s_buffers[name];
}

void AudioManager::delSound(const std::string& name)
{
	s_buffers.erase(name);
}

sf::SoundBuffer& AudioManager::getSound(const std::string& name)
{
	return s_buffers[name];
}

void AudioManager::clearSounds()
{
	s_buffers.clear();
}


sf::Music& AudioManager::addEffect(const std::string& name, const std::string& path, bool forceReset)
{
	if (!hasEffect(name) || forceReset)
	{
		MNG_ASSERT_BASIC(s_effects[name].openFromFile(path));
		s_effects[name].setVolume(s_evol);
	}

	s_nameCache.push_back(name);

	return s_effects[name];
}

void AudioManager::delEffect(const std::string& name)
{
	s_effects.erase(name);
}

sf::Music& AudioManager::getEffect(const std::string& name)
{
	return s_effects[name];
}

void AudioManager::clearEffects()
{
	s_effects.clear();
}


void AudioManager::clearNonPersisting()
{
	std::vector<std::string> toBeErased;

	for (auto& [name, _] : s_effects)
	{
		if (std::find(s_nameCache.begin(), s_nameCache.end(), name) == s_nameCache.end())
			toBeErased.push_back(name);
	}

	for (const std::string& str : toBeErased)
		s_effects.erase(str);

	s_nameCache.clear();
}

bool AudioManager::hasEffect(const std::string& name)
{
	return s_effects.find(name) != s_effects.end();
}


void AudioManager::setGlobalVolume(float volume)
{
	s_gvol = volume - 100 * (int)(volume * 0.01f);
	sf::Listener::setGlobalVolume(s_gvol);
}

void AudioManager::setMusicVolume(float volume)
{
	s_mvol = volume - 100 * (int)(volume * 0.01f);
	track.setVolume(s_mvol);
}

void AudioManager::setEffectVolume(float volume)
{
	s_evol = volume - 100 * (int)(volume * 0.01f);
	for (auto& [_, effect] : s_effects)
		effect.setVolume(s_svol);
}

void AudioManager::setSoundVolume(float volume)
{
	s_svol = volume - 100 * (int)(volume * 0.01f);
}


float AudioManager::getGlobalVolume()
{
	return s_gvol;
}

float AudioManager::getMusicVolume()
{
	return s_mvol;
}

float AudioManager::getEffectVolume()
{
	return s_evol;
}

float AudioManager::getSoundVolume()
{
	return s_svol;
}


/*
std::vector<std::future<void>> AudioManager::s_fadeFutures;

void AudioManager::_fadeAll(float delay)
{
	float maximum = sf::Listener::getGlobalVolume();

	for (float i = maximum; i > 0; i -= maximum / 100)
	{
		sf::Listener::setGlobalVolume(i);
		sf::sleep(sf::milliseconds((int)(delay * 10)));
	}

	sf::Listener::setGlobalVolume(0);

	track.stop();
	for (auto& layer : s_effects)
		layer.second.stop();
}

void AudioManager::_fadeTrack(const std::string& name, float delay)
{
	sf::Music& t = name == "main" ? track : getEffectLayer(name);
	float maximum = t.getVolume();

	for (float i = maximum; i > 0; i -= maximum / 100)
	{
		t.setVolume(i);
		sf::sleep(sf::milliseconds((int)(delay * 10)));
	}

	if (name != "main")
		deleteEffectLayer(name);
	else
		t.stop();
}
*/