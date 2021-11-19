#pragma once

#include "gui/backend/TextWidget.h"


namespace gui
{

class Dialogue : public TextWidget
{
public:
	using Ptr = std::shared_ptr<Dialogue>;

private:
	std::vector<std::string> m_alltext;
	std::string m_curtext = "";

	size_t m_pageindex = 0;
	size_t m_charindex = 0;

	float m_chartime = 0.f;
	float m_totaltime = 0.f;

public:
	static Ptr create()
	{
		return std::make_shared<Dialogue>();
	}

	bool isPlaying();
	bool isTerminated();

	void load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts) {}
	void loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset) {}

	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
};

}