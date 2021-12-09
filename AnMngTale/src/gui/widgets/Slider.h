#pragma once

#include "gui/backend/Widget.h"
#include "gui/backend/Callback.h"


namespace gui
{

class Slider : public Widget
{
public:
	using Ptr = std::shared_ptr<Slider>;

	Callback onValueChange;

private:
	sf::Vertex m_line[2];
	sf::RectangleShape shape;
	sf::IntRect m_buttonBounds;

	float* m_target = nullptr;
	float m_value = 0.f;
	float m_scale = 1.f;
	uint32_t m_width = 0U;
	bool m_held = false;

private:
	bool slide(int x);

public:
	static Ptr create()
	{
		return std::make_shared<Slider>();
	}

	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts) override;
	void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset) override;

	void setRawValue(float val);
	float getRawValue() const;

	void setScaleTarget(float& target, float scale);
	void setLayout(sf::Vector2i position, sf::Vector2i size) override;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}