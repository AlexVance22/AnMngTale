#pragma once

#include "graphics/Sprite.h"

using TextureMap = std::unordered_map<std::string, sf::Texture>;
using FontMap = std::unordered_map<std::string, sf::Font>;


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
	virtual void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts) = 0;
	virtual void load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& preset) = 0;

	virtual void setLayout(sf::Vector2i position, sf::Vector2i size);

	void setEnabled(bool enabled);
	bool isEnabled() const;

	void setVisible(bool visible);
	bool isVisible() const;

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