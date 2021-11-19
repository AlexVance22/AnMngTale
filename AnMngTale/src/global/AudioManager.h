#pragma once


class AudioManager
{
private:
	static std::unordered_map<std::string, sf::SoundBuffer> s_buffers;
	static std::unordered_map<std::string, sf::Music> s_effects;

	static std::vector<std::future<void>> s_fadeFutures;

	static void _fadeAll(float delay);
	static void _fadeTrack(const std::string& track, float delay);

public:
	static sf::Music track;

	static void initTrack(const std::string& path, bool loop);

	static sf::Music& addEffectLayer(const std::string& name, const std::string& path, bool loop);
	static void deleteEffectLayer(const std::string& name);
	static void clearEffects();
	static void setEffectVolume(float volume);

	static void addSound(const std::string& name, const std::string& path);
	static void deleteSound(const std::string& name);

	static sf::Music& getEffectLayer(const std::string& name) { return s_effects[name]; }
	static sf::SoundBuffer& getSound(const std::string& name) { return s_buffers[name]; }

	static void fadeOut(float delay) { s_fadeFutures.push_back(std::async(std::launch::async, _fadeAll, delay)); }
	static void fadeOut(const std::string& name, float delay) { s_fadeFutures.push_back(std::async(std::launch::async, _fadeTrack, name, delay)); };

	static void clearFutures() { s_fadeFutures.clear(); }
};