#pragma once

#include "gui/backend/Widget.h"


namespace gui
{

class Container : public Widget
{
public:
	using Ptr = std::shared_ptr<Container>;

private:
	std::unordered_map<std::string, Widget::Ptr> m_widgets;

	sf::RectangleShape m_block;

public:
	static Ptr create()
	{
		return std::make_shared<Container>();
	}

	void add(Widget::Ptr widget, const std::string& name);
	const Widget::Ptr get(const std::string& name) const;
	Widget::Ptr get(const std::string& name);

	void load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts);
	void loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& presets);

	void handleEvent(const sf::Event& event) override;
	void update(const float deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

typedef Container Root;

}