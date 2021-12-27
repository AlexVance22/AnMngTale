#include "PCH.h"
#include "Label.h"

#include "core/Asserts.h"

#include "io/JsonUtils.h"


namespace gui
{

void Label::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts)
{
	m_text.setString(data["text"].GetString());

	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(data["size"]));

	MNG_ASSERT_SLIM(fonts.find(data["font"].GetString()) != fonts.end());
	setFont(fonts.at(data["font"].GetString()));
	setCharacterSize(data["fontsize"].GetUint());

	uint32_t style = 0;

	for (const auto& s : data["style"].GetArray())
		style |= s.GetUint();

	setStyle(style);
}

void Label::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset)
{
	m_text.setString(data["text"].GetString());

	setLayout(JsonToVec2<int>(data["position"]), JsonToVec2<int>(preset["size"]));

	MNG_ASSERT_SLIM(fonts.find(preset["font"].GetString()) != fonts.end());
	setFont(fonts.at(preset["font"].GetString()));
	setCharacterSize(preset["fontsize"].GetUint());

	uint32_t style = 0;

	for (const auto& s : preset["style"].GetArray())
		style |= s.GetUint();

	setStyle(style);
}

}