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

	sf::IntRect m_buttonBounds;

	sf::RectangleShape shape;

	float* m_target = nullptr;
	float m_value = 0.f;
	float m_scale = 1.f;
	int m_width = 0;
	bool m_held = false;

private:
	void slide(int x);

public:
	static Ptr create()
	{
		return std::make_shared<Slider>();
	}

	void setScaleTarget(float& target, float scale);
	void setLayout(sf::Vector2i position, sf::Vector2i size) override;

	void load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts);
	void loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset);

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}