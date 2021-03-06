#include "PCH.h"
#include "Container.h"

#include "core/Asserts.h"

#include "io/JsonUtils.h"

#include "../Label.h"
#include "../Button.h"
#include "../Slider.h"


namespace gui
{

void Container::add(Widget::Ptr widget, const std::string& name)
{
	m_widgets[name] = widget;
}

const Widget::Ptr Container::get(const std::string& name) const
{
	return m_widgets.at(name);
}

Widget::Ptr Container::get(const std::string& name)
{
	return m_widgets[name];
}


void Container::load(const rapidjson::Value& data, const TextureMap& textures, const FontMap& fonts, const rapidjson::Value& presets)
{
	m_view.setSize(JsonToVec2<float>(data["view"]));
	m_view.setCenter(JsonToVec2<float>(data["view"]) * 0.5f);

	const auto& tex = data["texture"];
	if (tex.IsNull())
	{
		m_block.setFillColor(JsonToColor(data["color"]));
		m_block.setSize(JsonToVec2<float>(data["size"]));
		m_block.setPosition(JsonToVec2<float>(data["pos"]));
	}
	else
	{
		MNG_ASSERT_SLIM(textures.find(tex.GetString()) != textures.end());
		m_block.setFillColor(sf::Color(0, 0, 0, 0));
		m_background.setTexture(textures.at(tex.GetString()));
		m_background.setPosition(JsonToVec2<float>(data["pos"]));
	}

	for (const auto& w : data["children"].GetObject())
	{
		const char* name = w.name.GetString();
		const auto& data = w.value;
		const char* type = data["type"].GetString();

		if (strncmp(type, "button", 32) == 0)
		{
			auto btn = gui::Button::create();
			if (data["preset"].IsNull())
				btn->load(data, textures, fonts);
			else
				btn->load(data, textures, fonts, presets["button"][data["preset"].GetString()]);
			m_widgets[name] = btn;
		}
		else if (strncmp(type, "label", 32) == 0)
		{
			auto lbl = gui::Label::create();
			if (data["preset"].IsNull())
				lbl->load(data, textures, fonts);
			else
				lbl->load(data, textures, fonts, presets["label"][data["preset"].GetString()]);
			m_widgets[name] = lbl;
		}
		else if (strncmp(type, "slider", 32) == 0)
		{
			auto slr = gui::Slider::create();
			if (data["preset"].IsNull())
				slr->load(data, textures, fonts);
			else
				slr->load(data, textures, fonts, presets["slider"][data["preset"].GetString()]);
			m_widgets[name] = slr;
		}
		else if (strncmp(type, "container", 32) == 0)
		{
			auto cnt = gui::Container::create();
			cnt->load(data, textures, fonts, presets);
			m_widgets[name] = cnt;
		}
		else
			MNG_ASSERT_MSG(false, "Invalid widget type");
	}
}


void Container::handleEvent(const sf::Event& event)
{
	for (auto [_, widget] : m_widgets)
	{
		if (widget->isEnabled())
			widget->handleEvent(event);
	}
}

void Container::update(const float deltaTime)
{
	for (auto [_, widget] : m_widgets)
	{
		if (widget->isEnabled())
			widget->update(deltaTime);
	}
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(m_view);

	target.draw(m_block);
	target.draw(m_background);

	for (auto [_, widget] : m_widgets)
	{
		if (widget->isVisible())
			target.draw(*widget);
	}
}

}