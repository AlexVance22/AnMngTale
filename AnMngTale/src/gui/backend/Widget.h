#pragma once

#include "graphics/Sprite.h"


namespace gui
{

class Widget : public sf::Drawable, public std::enable_shared_from_this<Widget>
{
public:
	using Ptr = std::shared_ptr<Widget>;

protected:
	sf::IntRect m_boundingBox;
	sf::Vector2f m_size;

	Sprite m_background;

	bool m_enabled = true;
	bool m_visible = true;
	bool m_hovered = false;

	virtual void onMouseHover();
	virtual void onMouseExit();

public:
	virtual void load(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts) = 0;
	virtual void loadPreset(const rapidjson::Value& data, const std::unordered_map<std::string, sf::Texture>& textures,
		const std::unordered_map<std::string, sf::Font>& fonts, const rapidjson::Value& preset) = 0;

	virtual void setLayout(sf::Vector2i position, sf::Vector2i size);

	void setEnabled(bool enabled) { m_enabled = enabled; }
	bool isEnabled() { return m_enabled; }

	void setVisible(bool visible) { m_visible = visible; }
	bool isVisible() { return m_visible; }

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(const float deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

template<typename _Widget, typename std::enable_if_t<std::is_base_of<gui::Widget, _Widget>::value>* = nullptr>
std::shared_ptr<_Widget> cast(Widget::Ptr ptr)
{
	return std::dynamic_pointer_cast<_Widget>(ptr);
}

}