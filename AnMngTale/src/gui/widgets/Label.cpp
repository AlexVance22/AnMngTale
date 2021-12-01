#include "PCH.h"
#include "Label.h"

#include "core/Asserts.h"


namespace gui
{

void Label::load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
	const std::unordered_map<std::string, sf::Font>& fonts)
{
	m_text.setString(data["text"].GetString());
	const auto& pos = data["position"];
	const auto& size = data["size"];
	setLayout(sf::Vector2i(pos[0].GetInt(), pos[1].GetInt()),
			  sf::Vector2i(size[0].GetInt(), size[1].GetInt()));

	MNG_ASSERT_SLIM(fonts.find(data["font"].GetString()) != fonts.end());
	setFont(fonts.at(data["font"].GetString()), data["fontsize"].GetUint());
}

void Label::loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
	const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset)
{
	m_text.setString(data["text"].GetString());
	const auto& pos = data["position"];
	const auto& size = preset["size"];
	setLayout(sf::Vector2i(pos[0].GetInt(), pos[1].GetInt()),
			  sf::Vector2i(size[0].GetInt(), size[1].GetInt()));

	MNG_ASSERT_SLIM(fonts.find(preset["font"].GetString()) != fonts.end());
	setFont(fonts.at(preset["font"].GetString()), preset["fontsize"].GetUint());
}

}