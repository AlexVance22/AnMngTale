#pragma once


class AudioManager
{
private:
	static std::unordered_map<std::string, sf::SoundBuffer> s_buffers;
	static std::unordered_map<std::string, sf::Music> s_effects;

	static std::vector<std::string> s_nameCache;

	static float s_gvol;
	static float s_mvol;
	static float s_evol;
	static float s_svol;

public:
	static sf::Music track;
	static std::string trackTitle;

	static sf::SoundBuffer& addSound(const std::string& name, const std::string& path);
	static void delSound(const std::string& name);
	static sf::SoundBuffer& getSound(const std::string& name);
	static void clearSounds();

	static sf::Music& addEffect(const std::string& name, const std::string& path, bool forceReset = false);
	static void delEffect(const std::string& name);
	static sf::Music& getEffect(const std::string& name);
	static void clearEffects();

	static void clearNonPersisting();
	static bool hasEffect(const std::string& name);

	static void setGlobalVolume(float volume);
	static void setMusicVolume(float volume);
	static void setEffectVolume(float volume);
	static void setSoundVolume(float volume);

	static float getGlobalVolume();
	static float getMusicVolume();
	static float getEffectVolume();
	static float getSoundVolume();
};

/*
static std::vector<std::future<void>> s_fadeFutures;

static void _fadeAll(float delay);
static void _fadeTrack(const std::string& track, float delay);

static void fadeOut(float delay) { s_fadeFutures.push_back(std::async(std::launch::async, _fadeAll, delay)); }
static void fadeOut(const std::string& name, float delay) { s_fadeFutures.push_back(std::async(std::launch::async, _fadeTrack, name, delay)); };
*/