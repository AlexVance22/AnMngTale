#include "PCH.h"
#include "AudioManager.h"

#include "core/Asserts.h"

#include "Data.h"


std::unordered_map<std::string, sf::SoundBuffer> AudioManager::s_buffers;
std::unordered_map<std::string, sf::Music> AudioManager::s_effects;

std::vector<std::future<void>> AudioManager::s_fadeFutures;

sf::Music AudioManager::track;


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


void AudioManager::initTrack(const std::string& path, bool loop)
{
	sf::Listener::setGlobalVolume(Data::masterVolume);
	MNG_ASSERT_BASIC(track.openFromFile(path));
	track.setVolume(Data::musicVolume);
	track.setLoop(loop);
	track.play();
}


sf::Music& AudioManager::addEffectLayer(const std::string& name, const std::string& path, bool loop)
{
	MNG_ASSERT_BASIC(s_effects[name].openFromFile(path));
	s_effects[name].setLoop(loop);
	s_effects[name].setVolume(Data::soundVolume);

	return s_effects[name];
}

void AudioManager::deleteEffectLayer(const std::string& name)
{ 
	s_effects[name].stop();
	s_effects.erase(name); 
}

void AudioManager::clearEffects()
{
	s_effects.clear();
}

void AudioManager::setEffectVolume(float volume)
{
	for (auto& pair : s_effects)
		pair.second.setVolume(volume);
}


void AudioManager::addSound(const std::string& name, const std::string& path)
{
	s_buffers[name].loadFromFile(path);
}

void AudioManager::deleteSound(const std::string& name)
{
	s_buffers.erase(name);
}