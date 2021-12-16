#include "PCH.h"
#include "Button.h"

#include "global/AudioManager.h"

#include "core/Asserts.h"

#include "io/JsonUtils.h"


namespace gui
{

void Button::onMouseHover()
{
	m_background.setFrame(0);
	m_background.setAnimation(0);
	m_background.setIsPlaying(true);

	m_sound.play();
}

void Button::onMouseExit()
{
	m_background.setFrame(0);
	m_background.setAnimation(1);
	m_background.setIsPlaying(true);
}


void Button::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts)
{
	m_text.setString(data["text"].GetString());

	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(data["size"]));

	MNG_ASSERT_SLIM(fonts.find(data["font"].GetString()) != fonts.end());
	setFont(fonts.at(data["font"].GetString()), data["fontsize"].GetUint());

	if (!data["sound"].IsNull())
		m_sound.setBuffer(AudioManager::getSound(data["sound"].GetString()));

	MNG_ASSERT_SLIM(textures.find(data["texture"].GetString()) != textures.end());
	float scale = data["size"][0].GetFloat() / (textures.at(data["texture"].GetString()).getSize().x / data["animlen"].GetUint());
	m_background.setScale(scale, scale);
	m_background.setTexture(textures.at(data["texture"].GetString()));
	m_background.animate(data["animlen"].GetUint(), 2);
	m_background.setFrameRate(50.f);
}

void Button::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset)
{
	m_text.setString(data["text"].GetString());

	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(preset["size"]));

	MNG_ASSERT_SLIM(fonts.find(preset["font"].GetString()) != fonts.end());
	setFont(fonts.at(preset["font"].GetString()), preset["fontsize"].GetUint());

	if (!preset["sound"].IsNull())
		m_sound.setBuffer(AudioManager::getSound(preset["sound"].GetString()));

	MNG_ASSERT_SLIM(textures.find(preset["texture"].GetString()) != textures.end());
	float scale = preset["size"][0].GetFloat() / (textures.at(preset["texture"].GetString()).getSize().x / preset["animlen"].GetUint());
	m_background.setScale(scale, scale);
	m_background.setTexture(textures.at(preset["texture"].GetString()));
	m_background.animate(preset["animlen"].GetUint(), 2);
	m_background.setFrameRate(50.f);
}


void Button::setSound(const sf::SoundBuffer& buffer)
{
	m_sound.setBuffer(buffer);
}


void Button::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (m_boundingBox.contains(event.mouseButton.x, event.mouseButton.y))
			onClick();
		break;
	case sf::Event::MouseMoved:
		if (m_boundingBox.contains(event.mouseMove.x, event.mouseMove.y))
		{
			if (!m_hovered)
			{
				m_hovered = true;
				onMouseHover();
			}
		}
		else if (m_hovered)
		{
			m_hovered = false;
			onMouseExit();
		}
		break;
	}
}

}