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
	static Ptr create()
	{
		return std::make_shared<Button>();
	}

	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts) override;
	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset) override;

	void setSound(const sf::SoundBuffer& buffer);

	void handleEvent(const sf::Event& event);
};

}