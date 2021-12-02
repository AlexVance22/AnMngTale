#pragma once

#include "gui/backend/TextWidget.h"


namespace gui
{

class Label : public TextWidget
{
public:
	using Ptr = std::shared_ptr<Label>;

private:
	void handleEvent(const sf::Event& event) override {}

public:
	static Ptr create()
	{
		return std::make_shared<Label>();
	}

	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts) override;
	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset) override;
};

}