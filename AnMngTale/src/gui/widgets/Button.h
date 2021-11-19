#pragma once

#include "gui/backend/TextWidget.h"
#include "gui/backend/Callback.h"


namespace gui
{

class Button : public TextWidget
{
public:
	using Ptr = std::shared_ptr<Button>;

	Callback onClick;

private:
	sf::Sound m_sound;

	void onMouseHover() override;
	void onMouseExit() override;

public:
	Button() = default;

	static Ptr create()
	{
		return std::make_shared<Button>();
	}

	void setSound(const sf::SoundBuffer& buffer)
	{
		m_sound.setBuffer(buffer);
	}

	void load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts);
	void loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset);

	void handleEvent(const sf::Event& event);
};

}