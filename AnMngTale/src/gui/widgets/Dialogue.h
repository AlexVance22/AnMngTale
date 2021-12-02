#pragma once

#include "gui/backend/TextWidget.h"
#include "gui/backend/Callback.h"


namespace gui
{

class Dialogue : public TextWidget
{
public:
	using Ptr = std::shared_ptr<Dialogue>;

	Callback onFinish;

private:
	const std::vector<std::string>* m_alltext;

	size_t m_pageindex = 0;
	size_t m_charindex = 0;

	float m_chartime = 0.f;
	float m_totaltime = 0.f;

private:
	bool isEndOfPage() const;
	bool isEndOfScript() const;

public:
	static Ptr create()
	{
		return std::make_shared<Dialogue>();
	}

	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts) override;
	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset) override {}

	void setPlaySpeed(float charPerSec);
	void setText(const std::vector<std::string>& text);

	void handleEvent(const sf::Event& event);
	void update(const float deltaTime);
};

}